#ifndef CS_APP_FPS_COUNTER_H
#define CS_APP_FPS_COUNTER_H
#include <ctime>
#include <deque>

namespace cs::app {
// Utility class that registers new frame events and
// computes the average frame rate
class AppFpsCounter {
 public:
  explicit AppFpsCounter(size_t fps)
      : frame_rate_fps_(fps) {}

  // Should be called to register a new frame rendering
  // event. Saves the timestamp this function was called to
  // a size-limited queue.
  void newFrame() {
    while (timestamps_queue_.size() > maxQueueSize()) {
      timestamps_queue_.pop_front();
    }

    timestamps_queue_.push_back(time(nullptr));
  }

  // Computes the FPS over the duration of the sliding
  // window
  float fps() const {
    if (timestamps_queue_.size() < 2) {
      return -1;
    }
    time_t oldest_timestamp_sec = timestamps_queue_.front();
    time_t newest_frame_timestamp_sec =
        timestamps_queue_.back();

    time_t deltaInSec =
        newest_frame_timestamp_sec - oldest_timestamp_sec;
    return static_cast<float>(timestamps_queue_.size()) /
           static_cast<float>(deltaInSec);
  }

 private:
  size_t maxQueueSize() const {
    return frame_rate_fps_ * MAX_SAMPLING_SECONDS;
  }

  static constexpr size_t MAX_SAMPLING_SECONDS = 5;
  size_t frame_rate_fps_ = 0;
  std::deque<time_t> timestamps_queue_;
};
}  // namespace cs::app

#endif  // CS_APP_FPS_COUNTER_H