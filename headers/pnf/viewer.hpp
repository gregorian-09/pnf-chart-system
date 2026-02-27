/// \file viewer.hpp
/// \brief Viewer integration API.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef VIEWER_HPP
#define VIEWER_HPP

#include "chart.hpp"
#include "indicators.hpp"
#include <string>
#include <functional>
#include <memory>

namespace pnf {
    /**
     * Configuration for the interactive chart viewer window.
     */
    struct ViewerConfig {
        int window_width = 1200;
        int window_height = 800;
        std::string window_title = "P&F Chart Viewer";

        // Cell rendering
        int cell_width = 24;
        int cell_height = 24;
        int font_size = 16;

        // Colors (RGB format: 0xRRGGBB)
        uint32_t background_color = 0xFFFFFF;
        uint32_t grid_color = 0xE5E7EB;
        uint32_t axis_color = 0x374151;
        uint32_t x_color = 0x22C55E;
        uint32_t o_color = 0xEF4444;
        uint32_t support_color = 0x22C55E;
        uint32_t resistance_color = 0xEF4444;
        uint32_t signal_buy_color = 0x3B82F6;
        uint32_t signal_sell_color = 0xF97316;

        // Display options
        bool show_grid = true;
        bool show_price_axis = true;
        bool show_column_numbers = true;
        bool show_support_resistance = true;
        bool show_signals = true;
        bool show_patterns = true;
        bool show_indicators = true;

        // Margins
        int margin_left = 80;
        int margin_right = 20;
        int margin_top = 40;
        int margin_bottom = 60;

        // Interaction
        double zoom_min = 0.25;
        double zoom_max = 4.0;
        double zoom_step = 0.1;
        double pan_speed = 1.0;
    };

    /**
     * Event types for viewer callbacks.
     */
    enum class ViewerEvent {
        Close,
        Click,
        DoubleClick,
        KeyPress,
        Zoom,
        Pan,
        Refresh
    };

    /**
     * Event data passed to callbacks.
     */
    struct ViewerEventData {
        ViewerEvent event{};
        int x = 0;
        int y = 0;
        int column_index = -1;
        double price = 0.0;
        int key_code = 0;
        double zoom_level = 1.0;
    };

    /**
     * Callback type for viewer events.
     */
    using ViewerCallback = std::function<void(const ViewerEventData&)>;

    /**
     * Interactive chart viewer implementation interface.
     * This is the internal interface - users should use the Viewer class.
     */
    class IViewerImpl {
    public:
        virtual ~IViewerImpl() = default;
        virtual bool initialize(const ViewerConfig& config) = 0;
        virtual void set_chart(const Chart* chart) = 0;
        virtual void set_indicators(const Indicators* indicators) = 0;
        virtual void render() = 0;
        virtual void process_events() = 0;
        virtual bool is_open() const = 0;
        virtual void close() = 0;
        virtual void set_callback(ViewerCallback callback) = 0;
        virtual void set_zoom(double zoom) = 0;
        virtual double get_zoom() const = 0;
        virtual void set_pan(double x, double y) = 0;
        virtual void get_pan(double& x, double& y) const = 0;
        virtual void center_on_column(int column) = 0;
        virtual void center_on_latest() = 0;
        virtual void refresh() = 0;
    };

    /**
     * Factory for creating viewer implementations.
     * Different backends can be registered (SDL2, GLFW, native, etc.)
     */
    class ViewerFactory {
    public:
        static std::unique_ptr<IViewerImpl> create();
        static bool is_available();
        static std::string backend_name();
    };

    /**
     * Interactive window-based chart viewer.
     *
     * Opens a native window displaying the P&F chart with support for:
     * - Pan and zoom with mouse
     * - Real-time chart updates
     * - Support/resistance level display
     * - Signal markers
     * - Pattern highlighting
     * - Indicator overlays
     *
     * Example usage:
     * @code
     * Chart chart;
     * // ... populate chart with data ...
     *
     * Viewer viewer;
     * viewer.set_chart(chart);
     * viewer.show(); // Opens window, blocks until closed
     * @endcode
     *
     * For non-blocking operation:
     * @code
     * Viewer viewer;
     * viewer.set_chart(chart);
     * viewer.open();
     * while (viewer.is_open()) {
     *     viewer.update(); // Process events, re-render if needed
     *     // ... update chart data ...
     * }
     * @endcode
     */
    class Viewer {
    public:
        Viewer();
        explicit Viewer(const ViewerConfig& config);
        ~Viewer();

        // Non-copyable but movable
        Viewer(const Viewer&) = delete;
        Viewer& operator=(const Viewer&) = delete;
        Viewer(Viewer&&) noexcept;
        Viewer& operator=(Viewer&&) noexcept;

        /**
         * Check if windowed visualization is available on this system.
         * Returns false if no suitable backend (SDL2, etc.) is available.
         */
        static bool is_available();

        /**
         * Get the name of the rendering backend being used.
         */
        static std::string backend_name();

        /**
         * Configure the viewer before opening.
         */
        void configure(const ViewerConfig& config);
        ViewerConfig config() const { return config_; }

        /**
         * Set the chart to display.
         * The chart must remain valid while the viewer is open.
         */
        void set_chart(const Chart& chart);

        /**
         * Set the chart pointer (allows nullptr).
         */
        void set_chart(const Chart* chart);

        /**
         * Set indicators to display alongside the chart.
         * The indicators must remain valid while the viewer is open.
         */
        void set_indicators(const Indicators& indicators);

        /**
         * Set indicators pointer (allows nullptr).
         */
        void set_indicators(const Indicators* indicators);

        /**
         * Register a callback for viewer events.
         */
        void set_callback(const ViewerCallback& callback);

        /**
         * Open the viewer window (non-blocking).
         * Returns true if window opened successfully.
         */
        bool open();

        /**
         * Show the viewer window and block until it's closed.
         * Convenience method combining open() + run().
         */
        bool show();

        /**
         * Check if the viewer window is currently open.
         */
        bool is_open() const;

        /**
         * Process events and re-render the chart.
         * Call this in a loop for non-blocking operation.
         */
        void update() const;

        /**
         * Run the viewer event loop until the window is closed.
         */
        void run() const;

        /**
         * Close the viewer window.
         */
        void close();

        /**
         * Force a re-render of the chart.
         * Call this after chart data has been updated.
         */
        void refresh() const;

        /**
         * Set the zoom level (1.0 = 100%).
         */
        void set_zoom(double zoom);

        /**
         * Get the current zoom level.
         */
        double zoom() const;

        /**
         * Set the pan offset in pixels.
         */
        void set_pan(double x, double y);

        /**
         * Get the current pan offset.
         */
        void pan(double& x, double& y) const;

        /**
         * Center the view on a specific column.
         */
        void center_on_column(int column) const;

        /**
         * Center the view on the most recent column.
         */
        void center_on_latest() const;

        /**
         * Reset zoom and pan to default (fit chart in window).
         */
        void reset_view();

    private:
        ViewerConfig config_;
        std::unique_ptr<IViewerImpl> impl_;
        const Chart* chart_ = nullptr;
        const Indicators* indicators_ = nullptr;
        ViewerCallback callback_;
        double zoom_ = 1.0;
        double pan_x_ = 0.0;
        double pan_y_ = 0.0;
    };
}

#endif //VIEWER_HPP
