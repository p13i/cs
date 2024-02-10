#include <stdint.h>
#include <stdio.h>

#include <functional>
#include <sstream>
#include <tuple>
#include <vector>

#include "cs/app/scene1.hh"
#include "cs/app/scene_animator.hh"
#include "cs/http/request.hh"
#include "cs/http/response.hh"
#include "cs/http/server.hh"
#include "cs/http/web_app.hh"
#include "cs/net/json/object.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"
#include "cs/result/result.hh"

using ::cs::app::SceneAnimator;
using ::cs::http::HTTP_200_OK;
using ::cs::http::HTTP_404_NOT_FOUND;
using ::cs::http::kContentTypeTextHtml;
using ::cs::http::kContentTypeTextJson;
using ::cs::http::Request;
using ::cs::http::Response;
using ::cs::http::WebApp;
using ::cs::net::json::Object;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

Response index(Request request) {
  std::stringstream ss;
  ss << "<h1>Welcome to my website!</h1>";
  ss << "<p>To code this app, I only used standard library "
        "methods (like `std::chrono` or `std::vector`) for "
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
  std::stringstream images_js_ss;
  images_js_ss << "[";
  for (size_t i = 0; i < frames.size(); i++) {
    const Film film = frames.at(i);
    images_js_ss << "[";
    for (size_t x = 0; x < film.width; x++) {
      images_js_ss << "[";
      for (size_t y = 0; y < film.height; y++) {
        const Pixel px = film.pixels[x][y];
        // Convert from pixel's data type (uint8_t) to an
        // int [https://stackoverflow.com/a/28414758]
        images_js_ss << "[" << +px.r << ", " << +px.g
                     << ", " << +px.b << ", " << +px.a
                     << "],";
      }
      images_js_ss << "],";
    }
    images_js_ss << "],";
  }
  images_js_ss << "]";

  // clang-format off
  std::stringstream ss;
  ss << "<p>Ray-tracer rendered " << frames.size() << " frames in " << render_time_ms << " ms.</p>";
  ss << R"html(
<canvas id="canvas" width=")html" << APP_SCREEN_WIDTH
  << R"html(" height=")html" << APP_SCREEN_HEIGHT 
  << R"html("></canvas>
<p id="fps"></p>
<script type="text/javascript">
  const IMAGES = )html" << images_js_ss.str() << R"html(;
  const FPS = )html" << APP_FRAME_RATE_FPS << R"html(;
  var i = 0;
  var oneSecondStartMs = 0;
  var oneSecondEndMs = 0;
  var drawMs = 0;
  const canvas = document.getElementById('canvas');
  var ctx = canvas.getContext('2d');
  function drawImage() {
    const startMs = Date.now();
    if (i % FPS == 0) {
      oneSecondStartMs = startMs;
    }
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    for (var y = canvas.height - 1; y >= 0; y--) {
      for (var x = 0; x < canvas.width; x++) {
        var [r, g, b, a] = IMAGES[i][x][y];
        a = 1;
        ctx.fillStyle = `rgba( ${r} , ${g} , ${b}, ${a} )`;
        ctx.fillRect(x, y, 1, 1);
      }
    }
    const endMs = Date.now();
    if (i % FPS == (FPS - 1)) {
      oneSecondEndMs = endMs;
      const oneSceneSecondInMs = (oneSecondEndMs - oneSecondStartMs);
      const fps = (FPS * 1000) / oneSceneSecondInMs;
      const message = `Drawing at ${fps.toFixed(2)} fps.`;
      document.getElementById("fps").innerHTML = message;
    }
    i = (i + 1) % IMAGES.length;
    drawMs = endMs - startMs;
    queueDrawImage();
  }

  function queueDrawImage() {
    const delayMs = 1000 / FPS - drawMs;
    setTimeout(drawImage, delayMs);
  }

  document.addEventListener("DOMContentLoaded", function() {
    queueDrawImage();
  });
</script>
)html";
  // clang-format on

  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Response json(Request request) {
  Object* object =
      new Object(std::map<std::string, Object*>{
          {"key", new Object(std::string("value"))},
          {"key2", new Object(std::vector<Object*>{
                       new Object(true),
                       new Object(false),
                       new Object(std::string("hello")),
                       new Object(1.4f),
                   })}});
  std::stringstream ss;
  ss << object;
  return Response(HTTP_200_OK, kContentTypeTextJson,
                  ss.str());
}

Result RunMyWebApp() {
  WebApp app;
  // Routes.
  ENSURE_OK(app.Register("GET", "/", index));
  ENSURE_OK(app.Register("GET", "/render/", render));
  ENSURE_OK(app.Register("GET", "/json/", json));
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
