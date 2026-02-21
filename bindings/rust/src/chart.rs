//! chart module for Rust bindings.
use crate::ffi;
use crate::types::*;

/// Point & Figure chart for technical analysis.
pub struct Chart {
    ptr: *mut ffi::PnfChart,
}

impl Chart {
    /// Creates a new chart with default configuration.
    ///
    /// # Returns
    /// 
    pub fn new() -> Self {
        let ptr = unsafe { ffi::pnf_chart_create_default() };
        Self { ptr }
    }

    /// Creates a new chart with the specified configuration.
    ///
    /// # Arguments
    /// * `config` - 
    ///
    /// # Returns
    /// 
    pub fn with_config(config: ChartConfig) -> Self {
        let ffi_config = config.to_ffi();
        let ptr = unsafe { ffi::pnf_chart_create(&ffi_config) };
        Self { ptr }
    }

    /// Adds OHLC data to the chart.
    ///
    /// # Arguments
    /// * `&mut self` - 
    /// * `high` - 
    /// * `low` - 
    /// * `close` - 
    /// * `timestamp` - 
    ///
    /// # Returns
    /// 
    pub fn add_data(&mut self, high: f64, low: f64, close: f64, timestamp: i64) -> bool {
        unsafe { ffi::pnf_chart_add_data(self.ptr, high, low, close, timestamp) }
    }

    /// Adds a single price point to the chart.
    ///
    /// # Arguments
    /// * `&mut self` - 
    /// * `price` - 
    /// * `timestamp` - 
    ///
    /// # Returns
    /// 
    pub fn add_price(&mut self, price: f64, timestamp: i64) -> bool {
        unsafe { ffi::pnf_chart_add_price(self.ptr, price, timestamp) }
    }

    /// Adds OHLC data using the OHLC struct.
    ///
    /// # Arguments
    /// * `&mut self` - 
    /// * `ohlc` - 
    ///
    /// # Returns
    /// 
    pub fn add_ohlc(&mut self, ohlc: &OHLC) -> bool {
        let ffi_ohlc = ohlc.to_ffi();
        unsafe { ffi::pnf_chart_add_ohlc(self.ptr, &ffi_ohlc) }
    }

    /// Returns the total number of columns in the chart.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn column_count(&self) -> usize {
        unsafe { ffi::pnf_chart_column_count(self.ptr) }
    }

    /// Returns the number of X columns.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn x_column_count(&self) -> usize {
        unsafe { ffi::pnf_chart_x_column_count(self.ptr) }
    }

    /// Returns the number of O columns.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn o_column_count(&self) -> usize {
        unsafe { ffi::pnf_chart_o_column_count(self.ptr) }
    }

    /// Returns the current box size.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn box_size(&self) -> f64 {
        unsafe { ffi::pnf_chart_box_size(self.ptr) }
    }

    /// Returns the type of column at the specified index.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn column_type(&self, index: usize) -> ColumnType {
        let ct = unsafe { ffi::pnf_chart_column_type(self.ptr, index) };
        ColumnType::from_ffi(ct)
    }

    /// Returns the number of boxes in the specified column.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn column_box_count(&self, index: usize) -> usize {
        unsafe { ffi::pnf_chart_column_box_count(self.ptr, index) }
    }

    /// Returns the highest price in the specified column.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn column_highest(&self, index: usize) -> f64 {
        unsafe { ffi::pnf_chart_column_highest(self.ptr, index) }
    }

    /// Returns the lowest price in the specified column.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn column_lowest(&self, index: usize) -> f64 {
        unsafe { ffi::pnf_chart_column_lowest(self.ptr, index) }
    }

    /// Returns the price of a box at the specified column and box index.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `col_index` - 
    /// * `box_index` - 
    ///
    /// # Returns
    /// 
    pub fn box_price(&self, col_index: usize, box_index: usize) -> f64 {
        unsafe { ffi::pnf_chart_box_price(self.ptr, col_index, box_index) }
    }

    /// Returns the type of box at the specified column and box index.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `col_index` - 
    /// * `box_index` - 
    ///
    /// # Returns
    /// 
    pub fn box_type(&self, col_index: usize, box_index: usize) -> BoxType {
        let bt = unsafe { ffi::pnf_chart_box_type(self.ptr, col_index, box_index) };
        BoxType::from_ffi(bt)
    }

    /// Returns the marker string for a box at the specified position.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `col_index` - 
    /// * `box_index` - 
    ///
    /// # Returns
    /// 
    pub fn box_marker(&self, col_index: usize, box_index: usize) -> Option<String> {
        let ptr = unsafe { ffi::pnf_chart_box_marker(self.ptr, col_index, box_index) };
        let result = cstr_to_string(ptr);
        if !ptr.is_null() {
            unsafe { ffi::pnf_free_string(ptr) };
        }
        result
    }

    /// Checks if the chart has a bullish bias.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn has_bullish_bias(&self) -> bool {
        unsafe { ffi::pnf_chart_has_bullish_bias(self.ptr) }
    }

    /// Checks if the chart has a bearish bias.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn has_bearish_bias(&self) -> bool {
        unsafe { ffi::pnf_chart_has_bearish_bias(self.ptr) }
    }

    /// Checks if the price is above bullish support line.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `price` - 
    ///
    /// # Returns
    /// 
    pub fn is_above_bullish_support(&self, price: f64) -> bool {
        unsafe { ffi::pnf_chart_is_above_bullish_support(self.ptr, price) }
    }

    /// Checks if the price is below bearish resistance line.
    ///
    /// # Arguments
    /// * `&self` - 
    /// * `price` - 
    ///
    /// # Returns
    /// 
    pub fn is_below_bearish_resistance(&self, price: f64) -> bool {
        unsafe { ffi::pnf_chart_is_below_bearish_resistance(self.ptr, price) }
    }

    /// Clears all data from the chart.
    ///
    /// # Arguments
    /// * `&mut self` - 
    pub fn clear(&mut self) {
        unsafe { ffi::pnf_chart_clear(self.ptr) }
    }

    /// Returns a string representation of the chart.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn to_string(&self) -> Option<String> {
        let ptr = unsafe { ffi::pnf_chart_to_string(self.ptr) };
        let result = cstr_to_string(ptr);
        if !ptr.is_null() {
            unsafe { ffi::pnf_free_string(ptr) };
        }
        result
    }

    /// Returns an ASCII representation of the chart.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn to_ascii(&self) -> Option<String> {
        let ptr = unsafe { ffi::pnf_chart_to_ascii(self.ptr) };
        let result = cstr_to_string(ptr);
        if !ptr.is_null() {
            unsafe { ffi::pnf_free_string(ptr) };
        }
        result
    }

    /// Returns a JSON representation of the chart.
    ///
    /// # Arguments
    /// * `&self` - 
    ///
    /// # Returns
    /// 
    pub fn to_json(&self) -> Option<String> {
        let ptr = unsafe { ffi::pnf_chart_to_json(self.ptr) };
        let result = cstr_to_string(ptr);
        if !ptr.is_null() {
            unsafe { ffi::pnf_free_string(ptr) };
        }
        result
    }

    pub(crate) fn as_ptr(&self) -> *const ffi::PnfChart {
        self.ptr as *const ffi::PnfChart
    }
}

impl Default for Chart {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for Chart {
    fn drop(&mut self) {
        unsafe {
            ffi::pnf_chart_destroy(self.ptr);
        }
    }
}

unsafe impl Send for Chart {}
unsafe impl Sync for Chart {}
