#include <stdint.h>
#include <stdio.h>

#include <sstream>
#include <tuple>
#include <functional>
#include <vector>

#include "cs/app/scene_animator.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"
#include "server.hh"

#define APP_FRAME_RATE_FPS 60
#define APP_ANIMATION_DURATION_SEC 1
#define APP_ANIMATION_NUM_FRAMES \
  (APP_FRAME_RATE_FPS * APP_ANIMATION_DURATION_SEC)
#define APP_SCREEN_WIDTH 512
#define APP_SCREEN_HEIGHT 512

using ::cs::app::SceneAnimator;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;

void render() {
  std::tuple<unsigned int, unsigned int> film_dimensions(
      APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);
  SceneAnimator animator(APP_ANIMATION_NUM_FRAMES,
                         film_dimensions);

  std::vector<Film> frames;

  unsigned int render_time_ms =
      time_it([&frames, &animator]() {
        frames = animator.render_all_frames(
            [](unsigned int render_time_ms,
               unsigned int frame_num,
               unsigned int num_frames) {
              std::cout << "Rendered frame " << frame_num
                        << " of " << num_frames << " in "
                        << render_time_ms << " ms."
                        << std::endl;
            });
      });

  std::cout << "Total render time is " << render_time_ms << " ms." << std::endl;
}

int main() {
  auto server = cs::http::HttpServer("0.0.0.0", 8080);
  return server.startListening(render);
}
