#include <stdint.h>
#include <stdio.h>

#include <functional>
#include <sstream>
#include <tuple>
#include <vector>

#include "cs/app/scene_animator.hh"
#include "cs/http/request.hh"
#include "cs/http/response.hh"
#include "cs/http/server.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"
#include "cs/result/result.hh"

#define APP_FRAME_RATE_FPS 1
#define APP_ANIMATION_DURATION_SEC 1
#define APP_ANIMATION_NUM_FRAMES \
  (APP_FRAME_RATE_FPS * APP_ANIMATION_DURATION_SEC)
#define APP_SCREEN_WIDTH 512
#define APP_SCREEN_HEIGHT 512

using ::cs::app::SceneAnimator;
using ::cs::http::HTTP_200_OK;
using ::cs::http::HTTP_404_NOT_FOUND;
using ::cs::http::kContentTypeTextHtml;
using ::cs::http::Request;
using ::cs::http::Response;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

Response request_handler(Request request) {
  std::cout << "request_handler(request=" << request << ")"
            << std::endl;

  if (request.method() == "GET" &&
      request.path() == "/render/") {
    return render(request);
  }

  return Response(HTTP_404_NOT_FOUND);
}

Response render(Request request) {
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

  std::stringstream ss;
  ss << "Total render time is " << render_time_ms << " ms."
     << std::endl;

  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Result RunServer() {
  auto server = cs::http::Server("0.0.0.0", 8080);
  ENSURE_OK(server.startServer());
  ENSURE_OK(server.startListening(request_handler));
  return Ok();
}

int main() {
  Result result = RunServer();
  if (!result.ok()) {
    std::cerr << result.message() << std::endl;
  }
}
