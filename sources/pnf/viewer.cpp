/// \file viewer.cpp
/// \brief Viewer implementation.

//
// Created by gregorian-rayne on 13/01/2026.
//

#include "pnf/viewer.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>

#ifdef PNF_HAS_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

namespace pnf
{
#ifdef PNF_HAS_SDL2

    /**
     * SDL2-based viewer implementation.
     */
    class SDL2ViewerImpl final : public IViewerImpl {
    public:
        SDL2ViewerImpl() = default;
        ~SDL2ViewerImpl() override {
            close();
        }

        bool initialize(const ViewerConfig& config) override {
            config_ = config;

            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                return false;
            }

            if (TTF_Init() < 0) {
                SDL_Quit();
                return false;
            }

            window_ = SDL_CreateWindow(
                config_.window_title.c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                config_.window_width, config_.window_height,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
            );

            if (!window_) {
                TTF_Quit();
                SDL_Quit();
                return false;
            }

            renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (!renderer_) {
                SDL_DestroyWindow(window_);
                TTF_Quit();
                SDL_Quit();
                return false;
            }

            // Load a basic font
            font_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", config_.font_size);
            if (!font_) {
                font_ = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono.ttf", config_.font_size);
            }
            if (!font_) {
                font_ = TTF_OpenFont("/System/Library/Fonts/Menlo.ttc", config_.font_size);
            }
            if (!font_) {
                font_ = TTF_OpenFont(R"(C:\Windows\Fonts\consola.ttf)", config_.font_size);
            }

            is_open_ = true;
            return true;
        }

        void set_chart(const Chart* chart) override {
            chart_ = chart;
            needs_render_ = true;
        }

        void set_indicators(const Indicators* indicators) override {
            indicators_ = indicators;
            needs_render_ = true;
        }

        void render() override {
            if (!is_open_ || !renderer_) return;

            // Clear background
            set_color(config_.background_color);
            SDL_RenderClear(renderer_);

            if (chart_ && chart_->column_count() > 0) {
                render_chart();
            } else {
                render_empty();
            }

            SDL_RenderPresent(renderer_);
            needs_render_ = false;
        }

        void process_events() override {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    is_open_ = false;
                    if (callback_) {
                        ViewerEventData data;
                        data.event = ViewerEvent::Close;
                        callback_(data);
                    }
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                        event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        config_.window_width = event.window.data1;
                        config_.window_height = event.window.data2;
                        needs_render_ = true;
                        }
                    break;

                case SDL_MOUSEWHEEL:
                    handle_mouse_wheel(event.wheel);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    handle_mouse_button(event.button, true);
                    break;

                case SDL_MOUSEBUTTONUP:
                    handle_mouse_button(event.button, false);
                    break;

                case SDL_MOUSEMOTION:
                    handle_mouse_motion(event.motion);
                    break;

                case SDL_KEYDOWN:
                    handle_key(event.key);
                    break;
                default:
                    break;
                }
            }

            if (needs_render_) {
                render();
            }
        }

        [[nodiscard]] bool is_open() const override { return is_open_; }

        void close() override {
            is_open_ = false;

            if (font_) {
                TTF_CloseFont(font_);
                font_ = nullptr;
            }

            if (renderer_) {
                SDL_DestroyRenderer(renderer_);
                renderer_ = nullptr;
            }

            if (window_) {
                SDL_DestroyWindow(window_);
                window_ = nullptr;
            }

            TTF_Quit();
            SDL_Quit();
        }

        void set_callback(const ViewerCallback callback) override {
            callback_ = callback;
        }

        void set_zoom(const double zoom) override {
            zoom_ = std::clamp(zoom, config_.zoom_min, config_.zoom_max);
            needs_render_ = true;
        }

        [[nodiscard]] double get_zoom() const override { return zoom_; }

        void set_pan(const double x, const double y) override {
            pan_x_ = x;
            pan_y_ = y;
            needs_render_ = true;
        }

        void get_pan(double& x, double& y) const override {
            x = pan_x_;
            y = pan_y_;
        }

        void center_on_column(const int column) override {
            if (!chart_ || column < 0 || column >= static_cast<int>(chart_->column_count())) return;

            const double chart_width = config_.window_width - config_.margin_left - config_.margin_right;
            const double scaled_cell = config_.cell_width * zoom_;
            pan_x_ = -(column * scaled_cell - chart_width / 2);
            needs_render_ = true;
        }

        void center_on_latest() override {
            if (chart_ && chart_->column_count() > 0) {
                center_on_column(static_cast<int>(chart_->column_count()) - 1);
            }
        }

        void refresh() override {
            needs_render_ = true;
        }

    private:
        void set_color(const uint32_t color) const
        {
            const uint8_t r = (color >> 16) & 0xFF;
            const uint8_t g = (color >> 8) & 0xFF;
            const uint8_t b = color & 0xFF;
            SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
        }

        void render_text(const std::string& text, const int x, const int y, const uint32_t color, const bool center = false) const
        {
            if (!font_) return;

            const SDL_Color sdl_color = {
                static_cast<Uint8>((color >> 16) & 0xFF),
                static_cast<Uint8>((color >> 8) & 0xFF),
                static_cast<Uint8>(color & 0xFF),
                255
            };

            SDL_Surface* surface = TTF_RenderText_Blended(font_, text.c_str(), sdl_color);
            if (!surface) return;

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
            if (!texture) {
                SDL_FreeSurface(surface);
                return;
            }

            SDL_Rect rect;
            rect.w = surface->w;
            rect.h = surface->h;
            rect.x = center ? x - rect.w / 2 : x;
            rect.y = y;

            SDL_RenderCopy(renderer_, texture, nullptr, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }

        void render_empty() const
        {
            render_text("No chart data", config_.window_width / 2, config_.window_height / 2,
                        config_.axis_color, true);
        }

        void render_chart() const
        {
            const double box_size = chart_->current_box_size();

            // Price range
            double min_price = std::numeric_limits<double>::max();
            double max_price = std::numeric_limits<double>::lowest();

            for (size_t i = 0; i < chart_->column_count(); i++) {
                const Column* col = chart_->column(i);
                min_price = std::min(min_price, col->lowest_price());
                max_price = std::max(max_price, col->highest_price());
            }

            const int rows = static_cast<int>((max_price - min_price) / box_size) + 1;
            const int cols = static_cast<int>(chart_->column_count());

            const double scaled_cell_w = config_.cell_width * zoom_;
            const double scaled_cell_h = config_.cell_height * zoom_;

            if (config_.show_grid) {
                render_grid(cols, rows, scaled_cell_w, scaled_cell_h);
            }

            if (config_.show_price_axis) {
                render_price_axis(rows, max_price, box_size, scaled_cell_h);
            }

            if (config_.show_column_numbers) {
                render_column_numbers(cols, scaled_cell_w);
            }

            if (config_.show_support_resistance && indicators_) {
                render_support_resistance(max_price, box_size, scaled_cell_h);
            }

            // Render columns (X's and O's)
            for (size_t c = 0; c < chart_->column_count(); c++) {
                render_column(chart_->column(c), static_cast<int>(c), max_price, box_size, scaled_cell_w,
                              scaled_cell_h);
            }

            if (config_.show_signals && indicators_) {
                render_signals(max_price, box_size, scaled_cell_w, scaled_cell_h);
            }

            render_info_panel();
        }

        void render_grid(const int cols, const int rows, const double cell_w, const double cell_h) const
        {
            set_color(config_.grid_color);

            const int start_x = config_.margin_left + static_cast<int>(pan_x_);
            const int start_y = config_.margin_top + static_cast<int>(pan_y_);

            // Horizontal lines
            for (int r = 0; r <= rows; r++) {
                if (const int y = start_y + static_cast<int>(r * cell_h); y >= config_.margin_top && y <= config_.window_height - config_.margin_bottom) {
                    SDL_RenderDrawLine(renderer_, config_.margin_left, y,
                                       config_.window_width - config_.margin_right, y);
                }
            }

            // Vertical lines
            for (int c = 0; c <= cols; c++) {
                if (const int x = start_x + static_cast<int>(c * cell_w); x >= config_.margin_left && x <= config_.window_width - config_.margin_right) {
                    SDL_RenderDrawLine(renderer_, x, config_.margin_top,
                                       x, config_.window_height - config_.margin_bottom);
                }
            }
        }

        void render_price_axis(const int rows, const double max_price, const double box_size, const double cell_h) const
        {
            const int start_y = config_.margin_top + static_cast<int>(pan_y_);
            const int label_step = std::max(1, rows / 10);

            for (int r = 0; r <= rows; r += label_step) {
                const double price = max_price - r * box_size;

                if (const int y = start_y + static_cast<int>(r * cell_h); y >= config_.margin_top - 10 && y <= config_.window_height - config_.margin_bottom + 10) {
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(2) << price;
                    render_text(oss.str(), 5, y - 6, config_.axis_color);
                }
            }
        }

        void render_column_numbers(const int cols, const double cell_w) const
        {
            const int start_x = config_.margin_left + static_cast<int>(pan_x_);
            const int label_step = std::max(1, cols / 20);

            for (int c = 0; c < cols; c += label_step) {
                if (const int x = start_x + static_cast<int>(c * cell_w + cell_w / 2); x >= config_.margin_left && x <= config_.window_width - config_.margin_right) {
                    std::ostringstream oss;
                    oss << c;
                    render_text(oss.str(), x, config_.window_height - config_.margin_bottom + 5,
                               config_.axis_color, true);
                }
            }
        }

        void render_column(const Column* col, const int col_index, const double max_price,
                           const double box_size, const double cell_w, const double cell_h) const
        {
            const int start_x = config_.margin_left + static_cast<int>(pan_x_);
            const int start_y = config_.margin_top + static_cast<int>(pan_y_);

            const int x_center = start_x + static_cast<int>(col_index * cell_w + cell_w / 2);

            // Skip if column is outside visible area
            if (x_center < config_.margin_left - cell_w || x_center > config_.window_width) return;

            const char symbol = (col->type() == ColumnType::X) ? 'X' : 'O';
            const uint32_t color = (col->type() == ColumnType::X) ? config_.x_color : config_.o_color;

            for (size_t b = 0; b < col->box_count(); b++) {
                const Box* box = col->get_box_at(b);
                const double price = box->price();
                const int row = static_cast<int>((max_price - price) / box_size);
                const int y_center = start_y + static_cast<int>(row * cell_h + cell_h / 2);

                // Skip if outside visible area
                if (y_center < config_.margin_top - cell_h || y_center > config_.window_height) continue;

                std::string s(1, symbol);
                render_text(s, x_center, y_center - config_.font_size / 2, color, true);
            }
        }

        void render_support_resistance(const double max_price, const double box_size, const double cell_h) const
        {
            const SupportResistance* sr = indicators_->support_resistance();
            if (!sr) return;

            const int start_y = config_.margin_top + static_cast<int>(pan_y_);
            const int chart_width = config_.window_width - config_.margin_left - config_.margin_right;

            // Support levels (green dashed lines)
            for (const auto& level : sr->support_levels()) {
                const int row = static_cast<int>((max_price - level.price) / box_size);

                if (const int y = start_y + static_cast<int>(row * cell_h); y >= config_.margin_top && y <= config_.window_height - config_.margin_bottom) {
                    set_color(config_.support_color);
                    for (int x = config_.margin_left; x < config_.margin_left + chart_width; x += 10) {
                        SDL_RenderDrawLine(renderer_, x, y, std::min(x + 5, config_.margin_left + chart_width), y);
                    }
                }
            }

            // Resistance levels (red dashed lines)
            for (const auto& level : sr->resistance_levels()) {
                const int row = static_cast<int>((max_price - level.price) / box_size);

                if (const int y = start_y + static_cast<int>(row * cell_h); y >= config_.margin_top && y <= config_.window_height - config_.margin_bottom) {
                    set_color(config_.resistance_color);
                    for (int x = config_.margin_left; x < config_.margin_left + chart_width; x += 10) {
                        SDL_RenderDrawLine(renderer_, x, y, std::min(x + 5, config_.margin_left + chart_width), y);
                    }
                }
            }
        }

        void render_signals(const double max_price, const double box_size, const double cell_w, const double cell_h) const
        {
            const SignalDetector* sig = indicators_->signals();
            if (!sig) return;

            const int start_x = config_.margin_left + static_cast<int>(pan_x_);
            const int start_y = config_.margin_top + static_cast<int>(pan_y_);

            for (const auto& signal : sig->signals()) {
                const int x = start_x + static_cast<int>(signal.column_index * cell_w + cell_w / 2);
                const int row = static_cast<int>((max_price - signal.price) / box_size);

                if (const int y = start_y + static_cast<int>(row * cell_h + cell_h / 2); x >= config_.margin_left && x <= config_.window_width - config_.margin_right &&
                    y >= config_.margin_top && y <= config_.window_height - config_.margin_bottom) {

                    const uint32_t color = (signal.type == SignalType::Buy) ?
                                     config_.signal_buy_color : config_.signal_sell_color;

                    // Draw a small triangle marker
                    set_color(color);
                    const int size = static_cast<int>(cell_h / 3);
                    if (signal.type == SignalType::Buy) {
                        // Upward triangle below the box
                        SDL_RenderDrawLine(renderer_, x, y + size, x - size, y + size * 2);
                        SDL_RenderDrawLine(renderer_, x - size, y + size * 2, x + size, y + size * 2);
                        SDL_RenderDrawLine(renderer_, x + size, y + size * 2, x, y + size);
                    } else {
                        // Downward triangle above the box
                        SDL_RenderDrawLine(renderer_, x, y - size, x - size, y - size * 2);
                        SDL_RenderDrawLine(renderer_, x - size, y - size * 2, x + size, y - size * 2);
                        SDL_RenderDrawLine(renderer_, x + size, y - size * 2, x, y - size);
                    }
                    }
            }
        }

        void render_info_panel() const
        {
            constexpr int y = 5;
            const int x = config_.margin_left;

            // Chart info
            std::ostringstream oss;
            oss << "Columns: " << chart_->column_count()
                << "  Box Size: " << std::fixed << std::setprecision(2) << chart_->current_box_size()
                << "  Zoom: " << static_cast<int>(zoom_ * 100) << "%";
            render_text(oss.str(), x, y, config_.axis_color);

            // Indicator info if available
            if (indicators_) {
                oss.str("");
                oss << "Signal: " << (indicators_->signals()->has_buy_signal() ? "BUY" :
                                      indicators_->signals()->has_sell_signal() ? "SELL" : "NONE")
                    << "  Patterns: " << indicators_->patterns()->pattern_count()
                    << " (" << indicators_->patterns()->bullish_count() << "B/"
                    << indicators_->patterns()->bearish_count() << "S)";
                render_text(oss.str(), config_.window_width / 2, y, config_.axis_color, true);
            }

            // Controls hint
            render_text("Mouse: Pan | Scroll: Zoom | R: Reset | Q: Quit",
                       config_.window_width - config_.margin_right, y, config_.axis_color);
        }

        void handle_mouse_wheel(const SDL_MouseWheelEvent& event) {
            const double old_zoom = zoom_;
            const double delta = event.y * config_.zoom_step;
            zoom_ = std::clamp(zoom_ + delta, config_.zoom_min, config_.zoom_max);

            // Zoom towards mouse position
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);

            if (zoom_ != old_zoom) {
                const double scale = zoom_ / old_zoom;
                pan_x_ = mouse_x - (mouse_x - pan_x_) * scale;
                pan_y_ = mouse_y - (mouse_y - pan_y_) * scale;
                needs_render_ = true;

                if (callback_) {
                    ViewerEventData data;
                    data.event = ViewerEvent::Zoom;
                    data.zoom_level = zoom_;
                    callback_(data);
                }
            }
        }

        void handle_mouse_button(const SDL_MouseButtonEvent& event, const bool down) {
            if (event.button == SDL_BUTTON_LEFT) {
                dragging_ = down;
                if (down) {
                    last_mouse_x_ = event.x;
                    last_mouse_y_ = event.y;
                }
            }

            if (down && callback_) {
                ViewerEventData data;
                data.event = (event.clicks == 2) ? ViewerEvent::DoubleClick : ViewerEvent::Click;
                data.x = event.x;
                data.y = event.y;

                // Calculate column and price from screen coordinates
                if (chart_ && chart_->column_count() > 0) {
                    const double scaled_cell_w = config_.cell_width * zoom_;
                    const double scaled_cell_h = config_.cell_height * zoom_;
                    const int start_x = config_.margin_left + static_cast<int>(pan_x_);
                    const int start_y = config_.margin_top + static_cast<int>(pan_y_);

                    data.column_index = static_cast<int>((event.x - start_x) / scaled_cell_w);

                    const double box_size = chart_->current_box_size();
                    double max_price = std::numeric_limits<double>::lowest();
                    for (size_t i = 0; i < chart_->column_count(); i++) {
                        max_price = std::max(max_price, chart_->column(i)->highest_price());
                    }
                    const int row = static_cast<int>((event.y - start_y) / scaled_cell_h);
                    data.price = max_price - row * box_size;
                }

                callback_(data);
            }
        }

        void handle_mouse_motion(const SDL_MouseMotionEvent& event) {
            if (dragging_) {
                const int dx = event.x - last_mouse_x_;
                const int dy = event.y - last_mouse_y_;
                pan_x_ += dx * config_.pan_speed;
                pan_y_ += dy * config_.pan_speed;
                last_mouse_x_ = event.x;
                last_mouse_y_ = event.y;
                needs_render_ = true;

                if (callback_) {
                    ViewerEventData data;
                    data.event = ViewerEvent::Pan;
                    data.x = static_cast<int>(pan_x_);
                    data.y = static_cast<int>(pan_y_);
                    callback_(data);
                }
            }
        }

        void handle_key(const SDL_KeyboardEvent& event) {
            switch (event.keysym.sym) {
            case SDLK_q:
            case SDLK_ESCAPE:
                is_open_ = false;
                break;

            case SDLK_r:
                zoom_ = 1.0;
                pan_x_ = 0;
                pan_y_ = 0;
                needs_render_ = true;
                break;

            case SDLK_HOME:
                if (chart_ && chart_->column_count() > 0) {
                    center_on_column(0);
                }
                break;

            case SDLK_END:
                center_on_latest();
                break;

            case SDLK_PLUS:
            case SDLK_EQUALS:
                set_zoom(zoom_ + config_.zoom_step);
                break;

            case SDLK_MINUS:
                set_zoom(zoom_ - config_.zoom_step);
                break;

            case SDLK_LEFT:
                pan_x_ += config_.cell_width * zoom_;
                needs_render_ = true;
                break;

            case SDLK_RIGHT:
                pan_x_ -= config_.cell_width * zoom_;
                needs_render_ = true;
                break;

            case SDLK_UP:
                pan_y_ += config_.cell_height * zoom_;
                needs_render_ = true;
                break;

            case SDLK_DOWN:
                pan_y_ -= config_.cell_height * zoom_;
                needs_render_ = true;
                break;
            default:
                break;
            }

            if (callback_) {
                ViewerEventData data;
                data.event = ViewerEvent::KeyPress;
                data.key_code = event.keysym.sym;
                callback_(data);
            }
        }

        ViewerConfig config_;
        SDL_Window* window_ = nullptr;
        SDL_Renderer* renderer_ = nullptr;
        TTF_Font* font_ = nullptr;
        const Chart* chart_ = nullptr;
        const Indicators* indicators_ = nullptr;
        ViewerCallback callback_;
        bool is_open_ = false;
        bool needs_render_ = true;
        double zoom_ = 1.0;
        double pan_x_ = 0.0;
        double pan_y_ = 0.0;
        bool dragging_ = false;
        int last_mouse_x_ = 0;
        int last_mouse_y_ = 0;
    };

    std::unique_ptr<IViewerImpl> ViewerFactory::create() {
        return std::make_unique<SDL2ViewerImpl>();
    }

    bool ViewerFactory::is_available() {
        return true;
    }

    std::string ViewerFactory::backend_name() {
        return "SDL2";
    }

#else // No SDL2 available

    /**
     * Stub implementation when no windowing backend is available.
     */
    class StubViewerImpl final : public IViewerImpl {
    public:
        bool initialize(const ViewerConfig&) override { return false; }
        void set_chart(const Chart*) override {}
        void set_indicators(const Indicators*) override {}
        void render() override {}
        void process_events() override {}
        bool is_open() const override { return false; }
        void close() override {}
        void set_callback(ViewerCallback) override {}
        void set_zoom(double) override {}
        double get_zoom() const override { return 1.0; }
        void set_pan(double, double) override {}
        void get_pan(double& x, double& y) const override { x = 0; y = 0; }
        void center_on_column(int) override {}
        void center_on_latest() override {}
        void refresh() override {}
    };

    std::unique_ptr<IViewerImpl> ViewerFactory::create() {
        return std::make_unique<StubViewerImpl>();
    }

    bool ViewerFactory::is_available() {
        return false;
    }

    std::string ViewerFactory::backend_name() {
        return "none";
    }

#endif

    // ========== Viewer class implementation ==========

    Viewer::Viewer() : impl_(nullptr) {}

    Viewer::Viewer(const ViewerConfig& config) : config_(config), impl_(nullptr) {}

    Viewer::~Viewer() {
        close();
    }

    Viewer::Viewer(Viewer&& other) noexcept
        : config_(other.config_)
        , impl_(std::move(other.impl_))
        , chart_(other.chart_)
        , indicators_(other.indicators_)
        , callback_(std::move(other.callback_))
        , zoom_(other.zoom_)
        , pan_x_(other.pan_x_)
        , pan_y_(other.pan_y_)
    {
        other.chart_ = nullptr;
        other.indicators_ = nullptr;
    }

    Viewer& Viewer::operator=(Viewer&& other) noexcept {
        if (this != &other) {
            close();
            config_ = other.config_;
            impl_ = std::move(other.impl_);
            chart_ = other.chart_;
            indicators_ = other.indicators_;
            callback_ = std::move(other.callback_);
            zoom_ = other.zoom_;
            pan_x_ = other.pan_x_;
            pan_y_ = other.pan_y_;
            other.chart_ = nullptr;
            other.indicators_ = nullptr;
        }
        return *this;
    }

    bool Viewer::is_available() {
        return ViewerFactory::is_available();
    }

    std::string Viewer::backend_name() {
        return ViewerFactory::backend_name();
    }

    void Viewer::configure(const ViewerConfig& config) {
        config_ = config;
    }

    void Viewer::set_chart(const Chart& chart) {
        chart_ = &chart;
        if (impl_) impl_->set_chart(chart_);
    }

    void Viewer::set_chart(const Chart* chart) {
        chart_ = chart;
        if (impl_) impl_->set_chart(chart_);
    }

    void Viewer::set_indicators(const Indicators& indicators) {
        indicators_ = &indicators;
        if (impl_) impl_->set_indicators(indicators_);
    }

    void Viewer::set_indicators(const Indicators* indicators) {
        indicators_ = indicators;
        if (impl_) impl_->set_indicators(indicators_);
    }

    void Viewer::set_callback(const ViewerCallback& callback) {
        callback_ = callback;
        if (impl_) impl_->set_callback(callback);
    }

    bool Viewer::open() {
        if (impl_ && impl_->is_open()) return true;

        impl_ = ViewerFactory::create();
        if (!impl_->initialize(config_)) {
            impl_.reset();
            return false;
        }

        impl_->set_chart(chart_);
        impl_->set_indicators(indicators_);
        if (callback_) impl_->set_callback(callback_);
        impl_->set_zoom(zoom_);
        impl_->set_pan(pan_x_, pan_y_);
        impl_->render();

        return true;
    }

    bool Viewer::show() {
        if (!open()) return false;
        run();
        return true;
    }

    bool Viewer::is_open() const {
        return impl_ && impl_->is_open();
    }

    void Viewer::update() const
    {
        if (impl_ && impl_->is_open()) {
            impl_->process_events();
        }
    }

    void Viewer::run() const
    {
        while (impl_ && impl_->is_open()) {
            impl_->process_events();
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }

    void Viewer::close() {
        if (impl_) {
            impl_->close();
            impl_.reset();
        }
    }

    void Viewer::refresh() const
    {
        if (impl_) impl_->refresh();
    }

    void Viewer::set_zoom(const double zoom) {
        zoom_ = std::clamp(zoom, config_.zoom_min, config_.zoom_max);
        if (impl_) impl_->set_zoom(zoom_);
    }

    double Viewer::zoom() const {
        if (impl_) return impl_->get_zoom();
        return zoom_;
    }

    void Viewer::set_pan(const double x, const double y) {
        pan_x_ = x;
        pan_y_ = y;
        if (impl_) impl_->set_pan(x, y);
    }

    void Viewer::pan(double& x, double& y) const {
        if (impl_) {
            impl_->get_pan(x, y);
        } else {
            x = pan_x_;
            y = pan_y_;
        }
    }

    void Viewer::center_on_column(const int column) const
    {
        if (impl_) impl_->center_on_column(column);
    }

    void Viewer::center_on_latest() const
    {
        if (impl_) impl_->center_on_latest();
    }

    void Viewer::reset_view() {
        zoom_ = 1.0;
        pan_x_ = 0;
        pan_y_ = 0;
        if (impl_) {
            impl_->set_zoom(zoom_);
            impl_->set_pan(pan_x_, pan_y_);
            impl_->refresh();
        }
    }
}