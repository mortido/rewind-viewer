#pragma once
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "rewind_client.h"

namespace rewind_viewer {

template <typename Vec2T>
class Plot {
 public:
  Plot(const Vec2T& position, const Vec2T& scale, const Vec2T& origin)
      : position_(position)
      , scale_(scale)
      , origin_(origin)
      , axis_color_(0xFF000000)
      , marker_size_(0.1) {}

  int add_series(const std::string& name, uint32_t color) {
    series_.emplace_back(name, color);
    drawn_values_count_.emplace_back(0);
    return static_cast<int>(series_.size()) - 1;
  }

  void add_value(int series_id, const Vec2T& value) {
    if (series_id < 0 || series_id >= static_cast<int>(series_.size())) {
      throw std::out_of_range("Series ID does not exist");
    }

    Vec2T transformed{position_.x + (value.x - origin_.x) * scale_.x,
                      position_.y + (value.y - origin_.y) * scale_.y};
    series_[series_id].values.emplace_back(transformed);
    series_[series_id].original_values.emplace_back(value);
  }

  void add_value(int series_id, double x, double y) {
    Vec2T value{x, y};
    add_value(series_id, value);
  }

  void set_marker_size(double size) {
    marker_size_ = size;
  }

  void set_axis_color(uint32_t color) {
    axis_color_ = color;
  }

  void draw(RewindClient& rc, bool draw_markers = true, bool draw_popups = false) {
    if (series_.empty()) {
      return;
    }

    for (const auto& series : series_) {
      rc.polyline(series.values, series.color);

      for (size_t i = 0; i < series.values.size(); ++i) {
        if (draw_markers) {
          rc.circle(series.values[i], marker_size_, series.color, true);
        }
        if (draw_popups) {
          rc.popup_round(series.values[i], marker_size_, "%s(%.2f)=%.2f", series.name.c_str(),
                         series.original_values[i].x, series.original_values[i].y);
        }
      }
    }
  }

  void draw_new(RewindClient& rc, bool draw_markers = true) {
    if (series_.empty()) {
      return;
    }

    for (size_t series_id = 0; series_id < series_.size(); ++series_id) {
      const auto& series = series_[series_id];
      size_t start_index = drawn_values_count_[series_id];

      if (start_index < series.values.size()) {
        for (size_t i = start_index; i < series.values.size(); ++i) {
          if (draw_markers) {
            rc.circle(series.values[i], marker_size_, series.color, true);
          }
        }

        if (start_index>0){
          start_index--;
        }
        std::vector<Vec2T> new_values(series.values.begin() + start_index, series.values.end());
        rc.polyline(new_values, series.color);
        drawn_values_count_[series_id] = series.values.size();
      }
    }
  }

  void draw_axes_lines(RewindClient& rc, const Vec2T& min_v,const Vec2T& max_v ) {
    Vec2T x_axis_start{min_v.x, origin_.y};
    Vec2T x_axis_end{max_v.x, origin_.y};
    Vec2T y_axis_start{origin_.x, min_v.y};
    Vec2T y_axis_end{origin_.x, max_v.y};

    rc.line(Vec2T{position_.x + (x_axis_start.x - origin_.x) * scale_.x, position_.y},
            Vec2T{position_.x + (x_axis_end.x - origin_.x) * scale_.x, position_.y},
            axis_color_);  // Draw X axis

    rc.line(Vec2T{position_.x, position_.y + (y_axis_start.y - origin_.y) * scale_.y},
            Vec2T{position_.x, position_.y + (y_axis_end.y - origin_.y) * scale_.y},
            axis_color_);  // Draw Y axis
  }

 private:
  struct Series {
    Series(std::string  name, uint32_t color)
        : name(std::move(name))
        , color(color) {}

    std::string name;
    uint32_t color;
    std::vector<Vec2T> values;
    std::vector<Vec2T> original_values;
  };

  Vec2T position_;
  Vec2T scale_;
  Vec2T origin_;
  uint32_t axis_color_;
  double marker_size_;
  std::vector<Series> series_;
  std::vector<size_t> drawn_values_count_;
};

}  // namespace rewind_viewer
