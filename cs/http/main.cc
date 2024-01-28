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
#include "cs/http/web_app.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"
#include "cs/result/result.hh"

#define APP_FRAME_RATE_FPS 1
#define APP_ANIMATION_DURATION_SEC 1
#define APP_NUM_FRAMES \
  (APP_FRAME_RATE_FPS * APP_ANIMATION_DURATION_SEC)
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
using ::cs::http::WebApp;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

Response index(Request request) {
  std::stringstream ss;
  ss << "<h1>Welcome to my website!</h1>";
  ss << "<p>To code this app, I only used standard library "
        "methods (like std::chrono or std::vector) for "
        "this web app framework.</p>";
  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Response render(Request request) {
  // Build scene
  std::tuple<unsigned int, unsigned int> film_dimensions(
      APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);
  SceneAnimator animator(APP_ANIMATION_NUM_FRAMES,
                         film_dimensions);
  // Render out frames
  std::vector<Film> frames;
  const auto render_time_ms =
      cs::profiling::time_it([&frames, &animator]() {
        frames = animator.render_all_frames();
      });

  // Serialize first frame as a JavaScript 2D array.
  Film first_film = frames.at(0);
  std::stringstream image_js_ss;
  image_js_ss << "[";
  for (size_t x = 0; x < first_film.width; x++) {
    image_js_ss << "[";
    for (size_t y = 0; y < first_film.height; y++) {
      const Pixel px = first_film.pixels[x][y];
      // Convert from pixel's data type (uint8_t) to an int
      // [https://stackoverflow.com/a/28414758]
      image_js_ss << "[" << +px.r << ", " << +px.g << ", "
                  << +px.b << ", " << +px.a << "]";
    }
    image_js_ss << "],";
  }
  image_js_ss << "];";

  // clang-format off
  std::stringstream ss;
  ss << "<p>Ray-tracer rendered " << frames.size() << " frames in " << render_time_ms << " ms.</p>";
  ss << R"html(
<canvas id="canvas" width=")html" << first_film.width 
  << R"html("" height=")html" << first_film.height 
  << R"html("/>
<script type="text/javascript">
const IMAGE = )html" << image_js_ss.str() << R"html(
function drawImage() {
  const canvas = document.getElementById('canvas');
  ctx = canvas.getContext('2d');
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  for (var y = canvas.height; y >= 0; y--) {
    for (var x = 0; x < canvas.width; x++) {
      const [r, g, b, a] = IMAGE[x][y];
      ctx.fillStyle = `rgb( ${r} , ${g} , ${b} )`;
      ctx.fillRect(x, y, 1, 1);
    }
  }
}
document.addEventListener("DOMContentLoaded", function() {
  drawImage();
});
</script>
)html";
  // clang-format on

  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Result RunMyWebApp() {
  WebApp app;
  // Routes.
  ENSURE_OK(app.Register("GET", "/", index));
  ENSURE_OK(app.Register("GET", "/render/", render));
  // Run web app on host at port 8080.
  return app.RunServer("0.0.0.0", 8080);
}

int main() {
  Result result = RunMyWebApp();
  if (!result.ok()) {
    std::cerr << result << std::endl;
  }
  return result.code();
}
