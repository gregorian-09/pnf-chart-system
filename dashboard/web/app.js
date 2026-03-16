(function () {
  const cfg = window.PNF_DASHBOARD_CONFIG || {};
  const wsUrl = cfg.wsUrl || ((location.protocol === 'https:' ? 'wss://' : 'ws://') + location.host + '/ws');
  const state = {
    statusEl: document.getElementById('conn-status'),
    metaEl: document.getElementById('meta-line'),
    chartStatsEl: document.getElementById('chart-stats'),
    chartGridEl: document.getElementById('chart-grid'),
    chartPanelEl: document.querySelector('.chart-panel'),
    chartToggleEl: document.getElementById('chart-toggle'),
    tooltipEl: document.getElementById('chart-tooltip'),
    signalToggleEl: document.getElementById('toggle-signals'),
    patternToggleEl: document.getElementById('toggle-patterns'),
    levelToggleEl: document.getElementById('toggle-levels'),
    smaShortToggleEl: document.getElementById('toggle-sma-short'),
    smaMediumToggleEl: document.getElementById('toggle-sma-medium'),
    smaLongToggleEl: document.getElementById('toggle-sma-long'),
    bandUpperToggleEl: document.getElementById('toggle-band-upper'),
    bandMiddleToggleEl: document.getElementById('toggle-band-middle'),
    bandLowerToggleEl: document.getElementById('toggle-band-lower'),
    rsiToggleEl: document.getElementById('toggle-rsi'),
    legendEl: document.getElementById('overlay-legend'),
    seriesLegendEl: document.getElementById('series-legend'),
    seriesReadoutEl: document.getElementById('series-readout'),
    jumpPrevSignalEl: document.getElementById('jump-prev-signal'),
    jumpNextSignalEl: document.getElementById('jump-latest-signal'),
    jumpPrevPatternEl: document.getElementById('jump-prev-pattern'),
    jumpNextPatternEl: document.getElementById('jump-latest-pattern'),
    recentWindowEl: document.getElementById('recent-window'),
    timelineScrubberEl: document.getElementById('timeline-scrubber'),
    timelineLabelEl: document.getElementById('timeline-label'),
    rsiPanelEl: document.getElementById('rsi-panel'),
    rsiStripEl: document.getElementById('rsi-strip'),
    rsiLatestEl: document.getElementById('rsi-latest'),
    snapshot: null,
    currentChart: null,
    currentIndicators: null,
    overlaySummary: null,
    overlayFilters: {
      signalBuy: true,
      signalSell: true,
      patternBull: true,
      patternBear: true,
      levelSupport: true,
      levelResistance: true,
      targetBull: true,
      targetBear: true
    },
    navIndex: { signal: -1, pattern: -1 },
    activeColumn: null,
    chartCrosshairEl: null,
    chartGeometry: null,
    rsiCrosshairEl: null,
    rsiPointMap: new Map()
  };

  let pollTimer = null;
  let chartExpanded = false;
  let suppressScrubber = false;

  function syncChartToggle() {
    state.chartPanelEl.classList.toggle('expanded', chartExpanded);
    state.chartToggleEl.textContent = chartExpanded ? 'Collapse' : 'Expand';
  }

  function setStatus(label, cls) {
    state.statusEl.textContent = label;
    state.statusEl.className = 'badge ' + cls;
  }

  function escapeHtml(value) {
    return String(value)
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;')
      .replace(/'/g, '&#39;');
  }

  function fmt(value, digits) {
    const num = Number(value);
    return Number.isFinite(num) ? num.toFixed(digits) : '-';
  }

  function priceKey(value) {
    return Number(value).toFixed(8);
  }

  function infoKey(columnIndex, price) {
    return String(columnIndex) + '|' + priceKey(price);
  }

  function nearestPrice(prices, target) {
    if (!prices.length || !Number.isFinite(Number(target))) {
      return null;
    }
    let best = prices[0];
    let bestDiff = Math.abs(best - target);
    for (let i = 1; i < prices.length; i += 1) {
      const diff = Math.abs(prices[i] - target);
      if (diff < bestDiff) {
        best = prices[i];
        bestDiff = diff;
      }
    }
    return best;
  }

  function pushMap(map, key, value) {
    if (!map.has(key)) {
      map.set(key, []);
    }
    map.get(key).push(value);
  }


  function isSeriesEnabled(name) {
    switch (name) {
      case 'sma_short': return !!state.smaShortToggleEl.checked;
      case 'sma_medium': return !!state.smaMediumToggleEl.checked;
      case 'sma_long': return !!state.smaLongToggleEl.checked;
      case 'bollinger_upper': return !!state.bandUpperToggleEl.checked;
      case 'bollinger_middle': return !!state.bandMiddleToggleEl.checked;
      case 'bollinger_lower': return !!state.bandLowerToggleEl.checked;
      case 'rsi': return !!state.rsiToggleEl.checked;
      default: return false;
    }
  }

  function seriesLabel(name) {
    switch (name) {
      case 'sma_short': return 'SMA Short';
      case 'sma_medium': return 'SMA Medium';
      case 'sma_long': return 'SMA Long';
      case 'bollinger_upper': return 'Upper Band';
      case 'bollinger_middle': return 'Middle Band';
      case 'bollinger_lower': return 'Lower Band';
      case 'rsi': return 'RSI';
      default: return name;
    }
  }

  function seriesClass(name) {
    switch (name) {
      case 'sma_short': return 'sma-short';
      case 'sma_medium': return 'sma-medium';
      case 'sma_long': return 'sma-long';
      case 'bollinger_upper': return 'band-upper';
      case 'bollinger_middle': return 'band-middle';
      case 'bollinger_lower': return 'band-lower';
      case 'rsi': return 'rsi-chip';
      default: return '';
    }
  }

  function enabledSeriesValues(series, columnIndex) {
    const items = [];
    ['sma_short','sma_medium','sma_long','bollinger_upper','bollinger_middle','bollinger_lower','rsi'].forEach((name) => {
      if (!isSeriesEnabled(name)) {
        return;
      }
      const values = Array.isArray(series[name]) ? series[name] : [];
      const value = Number(values[columnIndex]);
      if (Number.isFinite(value)) {
        items.push({ name, value });
      }
    });
    return items;
  }
  function overlayAllowed(type, item) {
    switch (type) {
      case 'signal':
        return String(item.type).toLowerCase() === 'sell' ? state.overlayFilters.signalSell : state.overlayFilters.signalBuy;
      case 'pattern':
        return item.is_bullish ? state.overlayFilters.patternBull : state.overlayFilters.patternBear;
      case 'level':
        return String(item.type).toLowerCase().indexOf('support') >= 0 ? state.overlayFilters.levelSupport : state.overlayFilters.levelResistance;
      case 'target':
        return item.is_bullish ? state.overlayFilters.targetBull : state.overlayFilters.targetBear;
      default:
        return true;
    }
  }

  function renderRSI(indicators) {
    const series = (indicators && indicators.series) || {};
    const values = Array.isArray(series.rsi) ? series.rsi.filter((v) => Number.isFinite(Number(v))) : [];
    const enabled = state.rsiToggleEl.checked && values.length > 0;
    state.rsiPanelEl.classList.toggle('hidden', !enabled);
    state.rsiPointMap = new Map();
    if (!enabled) {
      state.rsiStripEl.innerHTML = '';
      state.rsiLatestEl.textContent = 'Latest: -';
      state.rsiCrosshairEl = null;
      return;
    }
    const latest = values[values.length - 1];
    state.rsiLatestEl.textContent = 'Latest: ' + fmt(latest, 2);
    state.rsiStripEl.innerHTML = '';

    const slice = values.slice(Math.max(0, values.length - 180));
    const width = Math.max(420, slice.length * 4);
    const height = 92;
    const padding = { top: 10, right: 10, bottom: 14, left: 10 };
    const plotWidth = Math.max(1, width - padding.left - padding.right);
    const plotHeight = Math.max(1, height - padding.top - padding.bottom);
    const step = slice.length > 1 ? plotWidth / (slice.length - 1) : 0;
    const offset = values.length - slice.length;
    const yFor = (value) => padding.top + ((100 - Number(value)) / 100) * plotHeight;

    const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
    svg.setAttribute('viewBox', '0 0 ' + width + ' ' + height);
    svg.setAttribute('class', 'rsi-svg');

    [30, 50, 70].forEach((level) => {
      const guide = document.createElementNS('http://www.w3.org/2000/svg', 'line');
      guide.setAttribute('x1', String(padding.left));
      guide.setAttribute('x2', String(width - padding.right));
      guide.setAttribute('y1', String(yFor(level)));
      guide.setAttribute('y2', String(yFor(level)));
      guide.setAttribute('class', 'rsi-guide' + (level === 50 ? ' mid' : ''));
      svg.appendChild(guide);
    });

    const path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
    const points = slice.map((value, index) => {
      const x = padding.left + index * step;
      const y = yFor(value);
      return { x, y, index: offset + index, value: Number(value) };
    });
    points.forEach((point) => state.rsiPointMap.set(point.index, point.x));
    const d = points.map((point, index) => (index === 0 ? 'M' : 'L') + point.x.toFixed(2) + ' ' + point.y.toFixed(2)).join(' ');
    path.setAttribute('d', d);
    path.setAttribute('class', 'rsi-line');
    svg.appendChild(path);

    const hitPath = document.createElementNS('http://www.w3.org/2000/svg', 'path');
    hitPath.setAttribute('d', d);
    hitPath.setAttribute('class', 'rsi-hit');
    hitPath.addEventListener('mouseenter', function () {
      path.classList.add('hovered');
    });
    hitPath.addEventListener('mousemove', function (event) {
      const rect = svg.getBoundingClientRect();
      const scaleX = rect.width ? width / rect.width : 1;
      const mouseX = (event.clientX - rect.left) * scaleX;
      let best = points[0];
      let bestDist = Math.abs(points[0].x - mouseX);
      for (let i = 1; i < points.length; i += 1) {
        const dist = Math.abs(points[i].x - mouseX);
        if (dist < bestDist) {
          best = points[i];
          bestDist = dist;
        }
      }
      setActiveColumn(best.index);
      showTooltipForInfo({
        columnIndex: best.index,
        price: best.value,
        box: null,
        signals: [],
        patterns: [],
        levels: [],
        objectives: [],
          seriesValues: [{ name: 'rsi', value: best.value }]
        }, event, 'RSI Hover');
    });
    hitPath.addEventListener('mouseleave', function () {
      path.classList.remove('hovered');
      state.tooltipEl.classList.add('hidden');
    });
    hitPath.addEventListener('click', function (event) {
      const rect = svg.getBoundingClientRect();
      const scaleX = rect.width ? width / rect.width : 1;
      const mouseX = (event.clientX - rect.left) * scaleX;
      let best = points[0];
      let bestDist = Math.abs(points[0].x - mouseX);
      for (let i = 1; i < points.length; i += 1) {
        const dist = Math.abs(points[i].x - mouseX);
        if (dist < bestDist) {
          best = points[i];
          bestDist = dist;
        }
      }
        showTooltipForInfo({
          columnIndex: best.index,
          price: best.value,
          box: null,
        signals: [],
          patterns: [],
          levels: [],
          objectives: [],
          seriesValues: [{ name: 'rsi', value: best.value }]
        }, event, 'RSI Hover');
        setActiveColumn(best.index);
      });
    svg.appendChild(hitPath);

    const crosshair = document.createElementNS('http://www.w3.org/2000/svg', 'line');
    crosshair.setAttribute('y1', String(padding.top));
    crosshair.setAttribute('y2', String(height - padding.bottom));
    crosshair.setAttribute('class', 'rsi-crosshair hidden');
    svg.appendChild(crosshair);
    state.rsiCrosshairEl = crosshair;

    const latestDot = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
    latestDot.setAttribute('cx', String(points[points.length - 1].x));
    latestDot.setAttribute('cy', String(points[points.length - 1].y));
    latestDot.setAttribute('r', '3.5');
    latestDot.setAttribute('class', 'rsi-dot' + (latest >= 70 ? ' overbought' : latest <= 30 ? ' oversold' : ''));
    svg.appendChild(latestDot);

    state.rsiStripEl.appendChild(svg);
    if (state.activeColumn !== null) {
      setActiveColumn(state.activeColumn);
    }
  }

  function buildOverlayMaps(chart, indicators, prices) {
    const signalMap = new Map();
    const patternMap = new Map();
    const objectiveMap = new Map();
    const rowLevelMap = new Map();
    const congestionMap = new Map();

    const rawSignals = Array.isArray(indicators.signals) ? indicators.signals : [];
    const rawPatterns = Array.isArray(indicators.patterns) ? indicators.patterns : [];
    const rawObjectives = Array.isArray(indicators.price_objectives) ? indicators.price_objectives : [];
    const rawSupportLevels = Array.isArray(indicators.support_levels) ? indicators.support_levels : [];
    const rawResistanceLevels = Array.isArray(indicators.resistance_levels) ? indicators.resistance_levels : [];
    const rawZones = Array.isArray(indicators.congestion_zones) ? indicators.congestion_zones : [];

    if (state.signalToggleEl.checked) {
      rawSignals.filter((item) => overlayAllowed('signal', item)).forEach((signal) => {
        const price = nearestPrice(prices, signal.price);
        if (price !== null) {
          pushMap(signalMap, infoKey(signal.column_index, price), signal);
        }
      });
    }

    if (state.patternToggleEl.checked) {
      rawPatterns.filter((item) => overlayAllowed('pattern', item)).forEach((pattern) => {
        const price = nearestPrice(prices, pattern.price);
        if (price === null) {
          return;
        }
        const start = Math.max(0, Number(pattern.start_column || 0));
        const end = Math.max(start, Number(pattern.end_column || pattern.start_column || 0));
        for (let col = start; col <= end && col < chart.columns.length; col += 1) {
          pushMap(patternMap, infoKey(col, price), Object.assign({ overlay_column: col }, pattern));
        }
      });
      rawObjectives.filter((item) => overlayAllowed('target', item)).forEach((objective) => {
        const price = nearestPrice(prices, objective.target);
        if (price !== null) {
          pushMap(objectiveMap, infoKey(Number(objective.column_index || 0), price), objective);
        }
      });
    }

    if (state.levelToggleEl.checked) {
      rawSupportLevels.concat(rawResistanceLevels).filter((item) => overlayAllowed('level', item)).forEach((level) => {
        const price = nearestPrice(prices, level.price);
        if (price !== null) {
          pushMap(rowLevelMap, priceKey(price), level);
        }
      });
      rawZones.forEach((zone) => {
        const low = Number(zone.low_price);
        const high = Number(zone.high_price);
        prices.forEach((price) => {
          if (price <= high && price >= low) {
            for (let col = Number(zone.start_column || 0); col <= Number(zone.end_column || 0) && col < chart.columns.length; col += 1) {
              pushMap(congestionMap, infoKey(col, price), zone);
            }
          }
        });
      });
    }

    return {
      signalMap,
      patternMap,
      objectiveMap,
      rowLevelMap,
      congestionMap,
      summary: {
        signals: rawSignals,
        patterns: rawPatterns,
        supportLevels: rawSupportLevels,
        resistanceLevels: rawResistanceLevels,
        objectives: rawObjectives,
        zones: rawZones
      }
    };
  }

  function tooltipMarkup(info) {
    const blocks = [];
    const primaryLabel = (!info.box && info.seriesValues && info.seriesValues.length) ? 'Value' : 'Price';
    blocks.push('<div class="tooltip-line strong">Column ' + escapeHtml(info.columnIndex) + ' · ' + primaryLabel + ' ' + escapeHtml(fmt(info.price, 2)) + '</div>');
    if (info.box) {
      const boxLabel = info.box.marker ? ('Month ' + escapeHtml(info.box.marker)) : escapeHtml(String(info.box.type || 'X').toUpperCase());
      blocks.push('<div class="tooltip-line">Box: ' + boxLabel + '</div>');
    }
    function tags(label, cls, items, textFn) {
      if (!items.length) {
        return;
      }
      blocks.push('<div class="tooltip-block"><span class="tooltip-k">' + label + '</span>' + items.map((item) => '<span class="tag ' + cls + '">' + escapeHtml(textFn(item)) + '</span>').join('') + '</div>');
    }
    tags('Signals', 'signal ' + (info.signals.some((item) => String(item.type).toLowerCase() === 'sell') ? 'sell' : 'buy'), info.signals, (item) => item.type + ' ' + fmt(item.price, 2));
    tags('Patterns', 'pattern ' + (info.patterns.some((item) => item.is_bullish) ? 'bull' : 'bear'), info.patterns, (item) => item.type);
    tags('Levels', 'level support', info.levels, (item) => item.type + ' ' + fmt(item.price, 2));
    tags('Targets', 'objective ' + (info.objectives.some((item) => item.is_bullish) ? 'bull' : 'bear'), info.objectives, (item) => fmt(item.target, 2));
    tags('Series', 'ma', info.seriesValues || [], (item) => seriesLabel(item.name) + ' ' + fmt(item.value, 2));
    return blocks.join('');
  }

  function renderLegend(summary) {
    const items = [
      { key: 'signalBuy', label: 'Buy Signals', count: summary.signals.filter((item) => String(item.type).toLowerCase() !== 'sell').length, cls: 'buy' },
      { key: 'signalSell', label: 'Sell Signals', count: summary.signals.filter((item) => String(item.type).toLowerCase() === 'sell').length, cls: 'sell' },
      { key: 'patternBull', label: 'Bull Patterns', count: summary.patterns.filter((item) => item.is_bullish).length, cls: 'bull' },
      { key: 'patternBear', label: 'Bear Patterns', count: summary.patterns.filter((item) => !item.is_bullish).length, cls: 'bear' },
      { key: 'levelSupport', label: 'Support', count: summary.supportLevels.length, cls: 'support' },
      { key: 'levelResistance', label: 'Resistance', count: summary.resistanceLevels.length, cls: 'resistance' },
      { key: 'targetBull', label: 'Bull Targets', count: summary.objectives.filter((item) => item.is_bullish).length, cls: 'target-bull' },
      { key: 'targetBear', label: 'Bear Targets', count: summary.objectives.filter((item) => !item.is_bullish).length, cls: 'target-bear' }
    ];
    state.legendEl.innerHTML = '';
    items.forEach((item) => {
      const button = document.createElement('button');
      button.type = 'button';
      button.className = 'legend-chip ' + item.cls + ((item.key && state.overlayFilters[item.key]) || !item.key ? ' active' : ' muted');
      if (item.key) {
        button.dataset.filter = item.key;
      } else {
        button.disabled = true;
      }
      button.innerHTML = '<span>' + escapeHtml(item.label) + '</span><strong>' + item.count + '</strong>';
      state.legendEl.appendChild(button);
    });
  }


  function renderSeriesLegend(indicators) {
    const series = (indicators && indicators.series) || {};
    const items = ['sma_short','sma_medium','sma_long','bollinger_upper','bollinger_middle','bollinger_lower','rsi'];
    state.seriesLegendEl.innerHTML = '';
    items.forEach((name) => {
      const enabled = isSeriesEnabled(name);
      const values = Array.isArray(series[name]) ? series[name] : [];
      const latest = values.length ? Number(values[values.length - 1]) : NaN;
      const chip = document.createElement('span');
      chip.className = 'legend-chip series ' + seriesClass(name) + (enabled ? ' active' : ' muted');
      chip.innerHTML = '<span>' + escapeHtml(seriesLabel(name)) + '</span><strong>' + (Number.isFinite(latest) ? escapeHtml(fmt(latest, 2)) : '-') + '</strong>';
      state.seriesLegendEl.appendChild(chip);
    });
  }

  function updateSeriesReadout() {
    const indicators = state.currentIndicators || {};
    const series = indicators.series || {};
    if (state.activeColumn === null || state.activeColumn === undefined) {
      state.seriesReadoutEl.textContent = 'Hover a column to inspect enabled indicator values.';
      return;
    }
    const values = enabledSeriesValues(series, state.activeColumn);
    if (!values.length) {
      state.seriesReadoutEl.textContent = 'Column ' + state.activeColumn + ' · no enabled indicator values';
      return;
    }
    state.seriesReadoutEl.textContent =
      'Column ' + state.activeColumn + ' · ' +
      values.map((item) => seriesLabel(item.name) + ' ' + fmt(item.value, 2)).join(' · ');
  }

  function setActiveColumn(columnIndex) {
    if (!Number.isInteger(columnIndex) || columnIndex < 0) {
      return;
    }
    state.activeColumn = columnIndex;
    updateSeriesReadout();

    state.chartGridEl.querySelectorAll('.grid-cell.active-col, .col-head.active-col').forEach((el) => el.classList.remove('active-col'));
    state.chartGridEl.querySelectorAll('.grid-cell[data-col-index="' + String(columnIndex) + '"]').forEach((el) => el.classList.add('active-col'));
    const heads = state.chartGridEl.querySelectorAll('.col-head');
    if (heads[columnIndex]) {
      heads[columnIndex].classList.add('active-col');
    }

    if (state.chartCrosshairEl && state.chartGeometry) {
      const x = state.chartGeometry.leftAxisWidth + columnIndex * state.chartGeometry.columnWidth + state.chartGeometry.columnWidth / 2;
      state.chartCrosshairEl.style.left = x + 'px';
      state.chartCrosshairEl.classList.remove('hidden');
    }

    if (state.rsiCrosshairEl) {
      const x = state.rsiPointMap.get(columnIndex);
      if (x !== undefined) {
        state.rsiCrosshairEl.setAttribute('x1', String(x));
        state.rsiCrosshairEl.setAttribute('x2', String(x));
        state.rsiCrosshairEl.classList.remove('hidden');
      } else {
        state.rsiCrosshairEl.classList.add('hidden');
      }
    }
  }

  function syncTimeline(chart) {
    const max = Math.max(0, chart && chart.columns ? chart.columns.length - 1 : 0);
    suppressScrubber = true;
    state.timelineScrubberEl.max = String(max);
    if (Number(state.timelineScrubberEl.value) > max) {
      state.timelineScrubberEl.value = String(max);
    }
    state.timelineLabelEl.textContent = 'Column ' + state.timelineScrubberEl.value + ' / ' + max;
    suppressScrubber = false;
  }

  function scrollToColumn(columnIndex, alignRatio) {
    if (columnIndex === null || columnIndex === undefined) {
      return;
    }
    const target = state.chartGridEl.querySelector('.grid-cell[data-col-index="' + String(columnIndex) + '"]');
    if (!target) {
      return;
    }
    const gridRect = state.chartGridEl.getBoundingClientRect();
    const cellRect = target.getBoundingClientRect();
    const ratio = typeof alignRatio === 'number' ? alignRatio : 0.35;
    state.chartGridEl.scrollLeft += (cellRect.left - gridRect.left) - Math.max(40, gridRect.width * ratio);
    suppressScrubber = true;
    state.timelineScrubberEl.value = String(columnIndex);
    state.timelineLabelEl.textContent = 'Column ' + columnIndex + ' / ' + state.timelineScrubberEl.max;
    suppressScrubber = false;
    target.classList.add('flash-focus');
    window.setTimeout(() => target.classList.remove('flash-focus'), 900);
  }

  function filteredEvents(type) {
    if (!state.overlaySummary) {
      return [];
    }
    if (type === 'signal') {
      return state.overlaySummary.signals.filter((item) => overlayAllowed('signal', item)).sort((a, b) => Number(a.column_index) - Number(b.column_index));
    }
    return state.overlaySummary.patterns.filter((item) => overlayAllowed('pattern', item)).sort((a, b) => Number(a.end_column || a.start_column || 0) - Number(b.end_column || b.start_column || 0));
  }

  function focusEvent(type, event, label) {
    if (!event) {
      return;
    }
    const col = type === 'signal' ? Number(event.column_index) : Number(event.end_column || event.start_column || 0);
    setActiveColumn(col);
    scrollToColumn(col);
  }

  function navigateEvents(type, direction) {
    const events = filteredEvents(type);
    if (!events.length) {
      return;
    }
    const next = state.navIndex[type] + direction;
    state.navIndex[type] = next < 0 ? events.length - 1 : next >= events.length ? 0 : next;
    focusEvent(type, events[state.navIndex[type]], type === 'signal' ? 'Signal Navigator' : 'Pattern Navigator');
  }

  function handleGridScroll() {
    if (suppressScrubber) {
      return;
    }
    const sample = state.chartGridEl.querySelector('.grid-cell');
    if (!sample) {
      return;
    }
    const width = sample.offsetWidth || 26;
    const approx = Math.max(0, Math.round((state.chartGridEl.scrollLeft - 86) / width));
    const max = Number(state.timelineScrubberEl.max || 0);
    const value = Math.min(max, approx);
    suppressScrubber = true;
    state.timelineScrubberEl.value = String(value);
    state.timelineLabelEl.textContent = 'Column ' + value + ' / ' + max;
    suppressScrubber = false;
  }

  function attachGridInteractions(table) {
    let activeCell = null;
    function clearHover() {
      if (activeCell) {
        activeCell.classList.remove('hovered');
      }
      activeCell = null;
      state.tooltipEl.classList.add('hidden');
    }
    table.addEventListener('mousemove', (event) => {
      const cell = event.target.closest('.grid-cell, .price-cell');
      if (!cell || !table.contains(cell) || !cell.__cellInfo) {
        clearHover();
        return;
      }
      if (activeCell && activeCell !== cell) {
        activeCell.classList.remove('hovered');
      }
      activeCell = cell;
      activeCell.classList.add('hovered');
      if (Number.isInteger(cell.__cellInfo.columnIndex)) {
        setActiveColumn(cell.__cellInfo.columnIndex);
      }
      showTooltipForInfo(cell.__cellInfo, event, 'Hover Details');
    });
    table.addEventListener('mouseleave', clearHover);
    table.addEventListener('click', (event) => {
      const cell = event.target.closest('.grid-cell');
      if (!cell || !table.contains(cell) || !cell.__cellInfo) {
        return;
      }
      if (Number.isInteger(cell.__cellInfo.columnIndex)) {
        setActiveColumn(cell.__cellInfo.columnIndex);
      }
    });
  }


  function priceToY(prices, price, headerHeight, rowHeight) {
    const value = Number(price);
    if (!prices.length || !Number.isFinite(value)) {
      return null;
    }
    if (prices.length === 1) {
      return headerHeight + rowHeight / 2;
    }
    if (value > prices[0] || value < prices[prices.length - 1]) {
      return null;
    }
    for (let i = 0; i < prices.length - 1; i += 1) {
      const high = prices[i];
      const low = prices[i + 1];
      if (value <= high && value >= low) {
        const span = high - low || 1;
        const t = (high - value) / span;
        return headerHeight + i * rowHeight + rowHeight / 2 + t * rowHeight;
      }
    }
    return headerHeight + (prices.length - 1) * rowHeight + rowHeight / 2;
  }

  function showTooltipForInfo(info, event, prefix) {
    state.tooltipEl.innerHTML = tooltipMarkup(info);
    state.tooltipEl.classList.remove('hidden');
    state.tooltipEl.style.left = event.clientX + 18 + 'px';
    state.tooltipEl.style.top = event.clientY + 18 + 'px';
  }

  function buildSeriesOverlay(chart, indicators, prices, width, height, metrics) {
    const series = (indicators && indicators.series) || {};
    const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
    svg.setAttribute('class', 'series-overlay');
    svg.setAttribute('viewBox', '0 0 ' + width + ' ' + height);
    svg.setAttribute('width', String(width));
    svg.setAttribute('height', String(height));

    const defs = document.createElementNS('http://www.w3.org/2000/svg', 'defs');
    const clip = document.createElementNS('http://www.w3.org/2000/svg', 'clipPath');
    clip.setAttribute('id', 'series-clip');
    const clipRect = document.createElementNS('http://www.w3.org/2000/svg', 'rect');
    clipRect.setAttribute('x', String(metrics.leftAxisWidth));
    clipRect.setAttribute('y', String(metrics.headerHeight));
    clipRect.setAttribute('width', String(metrics.columnWidth * chart.columns.length));
    clipRect.setAttribute('height', String(metrics.rowHeight * prices.length));
    clip.appendChild(clipRect);
    defs.appendChild(clip);
    svg.appendChild(defs);

    const group = document.createElementNS('http://www.w3.org/2000/svg', 'g');
    group.setAttribute('clip-path', 'url(#series-clip)');

    const palette = {
      sma_short: { cls: 'series-line sma-short', dot: 'series-dot sma-short' },
      sma_medium: { cls: 'series-line sma-medium', dot: 'series-dot sma-medium' },
      sma_long: { cls: 'series-line sma-long', dot: 'series-dot sma-long' },
      bollinger_upper: { cls: 'series-line band-upper', dot: 'series-dot band-upper' },
      bollinger_middle: { cls: 'series-line band-middle', dot: 'series-dot band-middle' },
      bollinger_lower: { cls: 'series-line band-lower', dot: 'series-dot band-lower' }
    };

    Object.keys(palette).forEach((name) => {
      if (!isSeriesEnabled(name)) {
        return;
      }
      const values = Array.isArray(series[name]) ? series[name] : [];
      const points = [];
      values.forEach((value, index) => {
        if (index >= chart.columns.length || !Number.isFinite(Number(value))) {
          return;
        }
        const x = metrics.leftAxisWidth + index * metrics.columnWidth + metrics.columnWidth / 2;
        const y = priceToY(prices, value, metrics.headerHeight, metrics.rowHeight);
        if (y !== null) {
          points.push({ x, y, index, value: Number(value) });
        }
      });
      if (!points.length) {
        return;
      }
      const segments = [];
      let currentSegment = [points[0]];
      for (let i = 1; i < points.length; i += 1) {
        if (points[i].index !== points[i - 1].index + 1) {
          if (currentSegment.length > 1) {
            segments.push(currentSegment);
          }
          currentSegment = [points[i]];
        } else {
          currentSegment.push(points[i]);
        }
      }
      if (currentSegment.length > 1) {
        segments.push(currentSegment);
      }
      if (!segments.length) {
        return;
      }

      function nearestSeriesPoint(event) {
        const rect = svg.getBoundingClientRect();
        const scaleX = rect.width ? width / rect.width : 1;
        const mouseX = (event.clientX - rect.left) * scaleX;
        let best = points[0];
        let bestDist = Math.abs(points[0].x - mouseX);
        for (let i = 1; i < points.length; i += 1) {
          const dist = Math.abs(points[i].x - mouseX);
          if (dist < bestDist) {
            best = points[i];
            bestDist = dist;
          }
        }
        return {
          columnIndex: best.index,
          price: best.value,
          box: null,
          signals: [],
          patterns: [],
          levels: [],
          objectives: [],
          seriesValues: [{ name, value: best.value }]
        };
      }

      segments.forEach((segment) => {
        const path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
        const d = segment.map((point, idx) => (idx === 0 ? 'M' : 'L') + point.x.toFixed(2) + ' ' + point.y.toFixed(2)).join(' ');
        path.setAttribute('d', d);
        path.setAttribute('class', palette[name].cls);
        group.appendChild(path);

        const hitPath = document.createElementNS('http://www.w3.org/2000/svg', 'path');
        hitPath.setAttribute('d', d);
        hitPath.setAttribute('class', 'series-hit ' + seriesClass(name));
        hitPath.addEventListener('mouseenter', function () {
          path.classList.add('hovered');
        });
        hitPath.addEventListener('mousemove', function (event) {
          path.classList.add('hovered');
          const info = nearestSeriesPoint(event);
          setActiveColumn(info.columnIndex);
          showTooltipForInfo(info, event, 'Series Hover');
        });
        hitPath.addEventListener('mouseleave', function () {
          path.classList.remove('hovered');
          state.tooltipEl.classList.add('hidden');
        });
        hitPath.addEventListener('click', function (event) {
          const info = nearestSeriesPoint(event);
          setActiveColumn(info.columnIndex);
        });
        group.appendChild(hitPath);
      });

      const dot = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
      const last = points[points.length - 1];
      dot.setAttribute('cx', String(last.x));
      dot.setAttribute('cy', String(last.y));
      dot.setAttribute('r', '3');
      dot.setAttribute('class', palette[name].dot);
      group.appendChild(dot);
    });

    svg.appendChild(group);
    return svg;
  }

  function renderChart(chart, indicators) {
    state.currentChart = chart;
    state.chartStatsEl.textContent = chart.column_count + ' columns · box ' + (chart.box_size ?? '-');
    state.chartGridEl.innerHTML = '';
    renderRSI(indicators || {});

    if (!chart.columns || !chart.columns.length) {
      state.chartCrosshairEl = null;
      state.chartGeometry = null;
      state.legendEl.innerHTML = '';
      state.seriesLegendEl.innerHTML = '';
      syncTimeline({ columns: [] });
      updateSeriesReadout();
      state.chartGridEl.innerHTML = '<div class="empty">No columns yet.</div>';
      return;
    }

    const prices = [];
    const seen = new Set();
    chart.columns.forEach((col) => {
      (col.boxes || []).forEach((box) => {
        const key = priceKey(box.price);
        if (!seen.has(key)) {
          seen.add(key);
          prices.push(Number(box.price));
        }
      });
    });
    prices.sort((a, b) => b - a);
    if (!prices.length) {
      state.chartCrosshairEl = null;
      state.chartGeometry = null;
      state.legendEl.innerHTML = '';
      state.seriesLegendEl.innerHTML = '';
      syncTimeline(chart);
      updateSeriesReadout();
      state.chartGridEl.innerHTML = '<div class="empty">No boxes yet.</div>';
      return;
    }

    const overlays = buildOverlayMaps(chart, indicators || {}, prices);
    state.overlaySummary = overlays.summary;
    renderLegend(overlays.summary);
    renderSeriesLegend(indicators || {});
    syncTimeline(chart);

    const metrics = { leftAxisWidth: 86, rightAxisWidth: 86, columnWidth: 26, rowHeight: 22, headerHeight: 22 };
    const stage = document.createElement('div');
    stage.className = 'chart-stage';
    const table = document.createElement('table');
    table.className = 'chart-table';
    const thead = document.createElement('thead');
    const headRow = document.createElement('tr');
    ['Price'].concat(chart.columns.map((c) => String(c.index))).concat(['Price']).forEach((label, idx) => {
      const th = document.createElement('th');
      if (idx === 0) th.className = 'price-head left';
      else if (idx === chart.columns.length + 1) th.className = 'price-head right';
      else th.className = 'col-head';
      th.textContent = label;
      headRow.appendChild(th);
    });
    thead.appendChild(headRow);
    table.appendChild(thead);

    const body = document.createElement('tbody');
    const columnMaps = chart.columns.map((col) => {
      const map = new Map();
      (col.boxes || []).forEach((box) => map.set(priceKey(box.price), box));
      return map;
    });

    prices.forEach((price) => {
      const tr = document.createElement('tr');
      const rowLevels = overlays.rowLevelMap.get(priceKey(price)) || [];
      const left = document.createElement('th');
      left.className = 'price-cell left' + (rowLevels.length ? ' annotated' : '');
      left.textContent = fmt(price, 2);
      left.__cellInfo = { columnIndex: 'Row', price, box: null, signals: [], patterns: [], levels: rowLevels, objectives: [], averages: [], bands: [], seriesValues: [] };
      tr.appendChild(left);

      columnMaps.forEach((map, colIndex) => {
        const key = infoKey(colIndex, price);
        const box = map.get(priceKey(price)) || null;
        const signals = overlays.signalMap.get(key) || [];
        const patterns = overlays.patternMap.get(key) || [];
        const objectives = overlays.objectiveMap.get(key) || [];
        const zones = overlays.congestionMap.get(key) || [];
        const seriesValues = enabledSeriesValues((indicators && indicators.series) || {}, colIndex);

        const td = document.createElement('td');
        td.className = 'grid-cell';
        td.dataset.colIndex = String(colIndex);
        if (zones.length) td.classList.add('zone');
        if (patterns.length) td.classList.add('pattern-cell');
        if (signals.length) td.classList.add('signal-cell');
        if (objectives.length) td.classList.add('objective-cell');

        const glyph = document.createElement('span');
        glyph.className = 'cell-glyph';
        if (box) {
          const text = (box.marker && String(box.marker).trim()) || (String(box.type || '').toUpperCase() === 'O' ? 'O' : 'X');
          glyph.textContent = text;
          td.classList.add(String(box.type || '').toUpperCase() === 'O' ? 'down' : 'up');
          if (box.marker && String(box.marker).trim()) td.classList.add('marker');
        }
        td.appendChild(glyph);

        if (signals.length || patterns.length || objectives.length) {
          const badgeWrap = document.createElement('div');
          badgeWrap.className = 'cell-overlays';
          if (signals.length) {
            const badge = document.createElement('span');
            badge.className = 'cell-flag signal ' + (signals.some((item) => String(item.type).toLowerCase() === 'sell') ? 'sell' : 'buy');
            badge.textContent = signals.length > 1 ? String(signals.length) : 'S';
            badgeWrap.appendChild(badge);
          }
          if (patterns.length) {
            const badge = document.createElement('span');
            badge.className = 'cell-flag pattern ' + (patterns.some((item) => item.is_bullish) ? 'bull' : 'bear');
            badge.textContent = patterns.length > 1 ? String(patterns.length) : 'P';
            badgeWrap.appendChild(badge);
          }
          if (objectives.length) {
            const badge = document.createElement('span');
            badge.className = 'cell-flag objective ' + (objectives.some((item) => item.is_bullish) ? 'bull' : 'bear');
            badge.textContent = 'T';
            badgeWrap.appendChild(badge);
          }
          td.appendChild(badgeWrap);
        }

        td.__cellInfo = { columnIndex: colIndex, price, box, signals, patterns, levels: rowLevels, objectives, seriesValues };
        tr.appendChild(td);
      });

      const right = document.createElement('th');
      right.className = 'price-cell right' + (rowLevels.length ? ' annotated' : '');
      right.textContent = fmt(price, 2);
      right.__cellInfo = left.__cellInfo;
      tr.appendChild(right);
      body.appendChild(tr);
    });

    table.appendChild(body);
    stage.appendChild(table);
    const totalWidth = metrics.leftAxisWidth + chart.columns.length * metrics.columnWidth + metrics.rightAxisWidth;
    const totalHeight = metrics.headerHeight + prices.length * metrics.rowHeight;
    stage.appendChild(buildSeriesOverlay(chart, indicators || {}, prices, totalWidth, totalHeight, metrics));
    const crosshair = document.createElement('div');
    crosshair.className = 'chart-crosshair hidden';
    crosshair.style.top = metrics.headerHeight + 'px';
    crosshair.style.height = prices.length * metrics.rowHeight + 'px';
    stage.appendChild(crosshair);
    state.chartCrosshairEl = crosshair;
    state.chartGeometry = metrics;
    state.chartGridEl.appendChild(stage);
    attachGridInteractions(table);
    state.chartGridEl.onscroll = handleGridScroll;
    if (Number.isInteger(state.activeColumn)) {
      setActiveColumn(state.activeColumn);
    } else {
      updateSeriesReadout();
    }
    fitRecentWindow();
  }

  function render(snapshot) {
    if (!snapshot || snapshot.type !== 'dashboard.snapshot' || !snapshot.payload) {
      return;
    }
    state.snapshot = snapshot;
    const meta = snapshot.payload.meta || {};
    state.metaEl.textContent = [meta.binding || 'unknown binding', 'seq ' + (snapshot.sequence ?? '-'), 'v' + (meta.library_version || '-')].join(' · ');
    const indicators = snapshot.payload.indicators || {};
    state.currentIndicators = indicators;
    renderChart(snapshot.payload.chart || { columns: [] }, indicators);
  }

  function rerenderCurrent() {
    if (state.snapshot) render(state.snapshot);
  }

  function fetchSnapshot() {
    fetch('/snapshot', { cache: 'no-store' }).then((res) => res.ok ? res.json() : null).then((data) => { if (data) render(data); }).catch(() => {});
  }

  function startPolling() {
    if (pollTimer) return;
    setStatus('Polling', 'warn');
    fetchSnapshot();
    pollTimer = window.setInterval(fetchSnapshot, 1000);
  }

  function stopPolling() {
    if (!pollTimer) return;
    window.clearInterval(pollTimer);
    pollTimer = null;
  }

  function connect() {
    setStatus('Connecting', 'warn');
    const ws = new WebSocket(wsUrl);
    ws.onopen = function () { stopPolling(); setStatus('Live', 'ok'); };
    ws.onclose = function () { startPolling(); window.setTimeout(connect, 1000); };
    ws.onerror = function () { startPolling(); };
    ws.onmessage = function (event) { try { render(JSON.parse(event.data)); } catch (err) { console.error('Invalid snapshot', err); } };
  }

  function fitRecentWindow() {
    if (!state.currentChart || !state.currentChart.columns || !state.currentChart.columns.length) return;
    const count = Number(state.recentWindowEl.value || 0);
    if (!count) {
      state.chartGridEl.scrollLeft = 0;
      handleGridScroll();
      return;
    }
    scrollToColumn(Math.max(0, state.currentChart.columns.length - count), 0.12);
  }

  state.chartToggleEl.addEventListener('click', function () { chartExpanded = !chartExpanded; syncChartToggle(); });
  [
    state.signalToggleEl,
    state.patternToggleEl,
    state.levelToggleEl,
    state.smaShortToggleEl,
    state.smaMediumToggleEl,
    state.smaLongToggleEl,
    state.bandUpperToggleEl,
    state.bandMiddleToggleEl,
    state.bandLowerToggleEl,
    state.rsiToggleEl
  ].forEach((el) => el.addEventListener('change', rerenderCurrent));
  state.legendEl.addEventListener('click', function (event) {
    const button = event.target.closest('.legend-chip');
    if (!button || button.disabled) return;
    const key = button.dataset.filter;
    if (!key || !(key in state.overlayFilters)) return;
    state.overlayFilters[key] = !state.overlayFilters[key];
    rerenderCurrent();
  });
  state.recentWindowEl.addEventListener('change', fitRecentWindow);
  state.timelineScrubberEl.addEventListener('input', function () {
    if (suppressScrubber) return;
    const value = Number(state.timelineScrubberEl.value || 0);
    state.timelineLabelEl.textContent = 'Column ' + value + ' / ' + state.timelineScrubberEl.max;
    scrollToColumn(value, 0.12);
  });
  state.jumpPrevSignalEl.addEventListener('click', function () { navigateEvents('signal', -1); });
  state.jumpNextSignalEl.addEventListener('click', function () { navigateEvents('signal', 1); });
  state.jumpPrevPatternEl.addEventListener('click', function () { navigateEvents('pattern', -1); });
  state.jumpNextPatternEl.addEventListener('click', function () { navigateEvents('pattern', 1); });
  document.addEventListener('keydown', function (event) {
    const tag = (event.target && event.target.tagName ? event.target.tagName : '').toLowerCase();
    if (tag === 'input' || tag === 'textarea' || tag === 'select') return;
    if (event.key === '[') { event.preventDefault(); navigateEvents('signal', -1); }
    else if (event.key === ']') { event.preventDefault(); navigateEvents('signal', 1); }
    else if (event.key === '{') { event.preventDefault(); navigateEvents('pattern', -1); }
    else if (event.key === '}') { event.preventDefault(); navigateEvents('pattern', 1); }
  });

  syncChartToggle();
  fetchSnapshot();
  connect();
})();
