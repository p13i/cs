#include <stdint.h>
#include <stdio.h>

#include <functional>
#include <optional>
#include <sstream>
#include <tuple>
#include <vector>

#include "cs/app/scene1.hh"
#include "cs/app/scene_animator.hh"
#include "cs/db/table.hh"
#include "cs/net/http/request.hh"
#include "cs/net/http/response.hh"
#include "cs/net/http/server.hh"
#include "cs/net/http/web_app.hh"
#include "cs/net/json/object.hh"
#include "cs/net/json/parsers.hh"
#include "cs/net/json/serialize.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"
#include "cs/result/result.hh"

using ::cs::app::SceneAnimator;
using ::cs::net::http::HTTP_200_OK;
using ::cs::net::http::HTTP_400_BAD_REQUEST;
using ::cs::net::http::HTTP_404_NOT_FOUND;
using ::cs::net::http::kContentTypeApplicationJson;
using ::cs::net::http::kContentTypeTextHtml;
using ::cs::net::http::Request;
using ::cs::net::http::Response;
using ::cs::net::http::WebApp;
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
  // Print sitemap of routes available.
  ss << "<h2>Routes</h2>";
  ss << "<ul>";
  ss << "<li><a href=\"/\">Home</a></li>";
  ss << "<li><a href=\"/render/\">Render</a></li>";
  ss << "<li><a href=\"/json/\">JSON</a></li>";
  ss << "<li><a href=\"/log/\">Log</a></li>";
  ss << "</ul>";
  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Response render(Request request) {
  uint width = APP_SCREEN_WIDTH;
  auto width_opt = request.get_query_param("width");
  if (width_opt.has_value()) {
    ASSIGN_OR_RETURN(width,
                     cs::net::json::parsers::ParseFloat(
                         width_opt.value()));
  }

  uint height = APP_SCREEN_HEIGHT;
  auto height_opt = request.get_query_param("height");
  if (height_opt.has_value()) {
    ASSIGN_OR_RETURN(height,
                     cs::net::json::parsers::ParseFloat(
                         height_opt.value()));
  }

  uint num_frames = 1;
  auto num_frames_opt =
      request.get_query_param("num_frames");
  if (num_frames_opt.has_value()) {
    ASSIGN_OR_RETURN(num_frames,
                     cs::net::json::parsers::ParseFloat(
                         num_frames_opt.value()));
  }

  // Build scene
  std::tuple<uint, uint> film_dimensions(width, height);
  SceneAnimator animator(num_frames, film_dimensions);
  // Render out frames
  std::vector<Film> frames;
  const auto render_time_ms =
      cs::profiling::time_it([&frames, &animator]() {
        frames = animator.render_all_frames();
      });

  // Serialize first frame as a JavaScript 2D array.
  std::vector<Object*> frames_json;
  for (size_t i = 0; i < frames.size(); i++) {
    const Film film = frames.at(i);
    std::vector<Object*> film_json;
    for (size_t x = 0; x < film.width; x++) {
      std::vector<Object*> column_json;
      column_json.reserve(film.height);
      for (size_t y = 0; y < film.height; y++) {
        const Pixel px = film.pixels[x][y];
        Object* pixel_json =
            new Object(std::vector<Object*>{
                new Object(static_cast<float>(px.r)),
                new Object(static_cast<float>(px.g)),
                new Object(static_cast<float>(px.b)),
                new Object(static_cast<float>(px.a))});
        column_json.push_back(pixel_json);
      }
      film_json.push_back(new Object(column_json));
    }
    frames_json.push_back(new Object(film_json));
  }

  Object* root_json = new Object(frames_json);

  // clang-format off
  std::stringstream ss;
  ss << "<p>Ray-tracer rendered " << frames.size() << " frames in "
    << render_time_ms << " ms at " << width << "x" << height << "px.</p>";
  ss << R"html(
<canvas id="canvas" width=")html" << width
  << R"html(" height=")html" << height 
  << R"html("></canvas>
<p id="fps"></p>
<form action="/render/" method="GET">
  <label for="width">Width:</label>
  <input type="number" id="width" name="width" value=")html" << width
  << R"html(">px
  <br/>
  <label for="height">Height:</label>
  <input type="number" id="height" name="height" value=")html" << height
  << R"html(">px
  <br/>
  <label for="num_frames">Number of frames:</label>
  <br/>
  <input type="number" id="num_frames" name="num_frames" value=")html" << num_frames
  << R"html(">
  <br/>
  <input type="submit" value="Render">
<script type="text/javascript">
  const IMAGES = )html" << root_json << R"html(;
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

  delete root_json;

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
  cs::net::json::SerializeObject(ss, object, 4);
  delete object;
  return Response(HTTP_200_OK, kContentTypeApplicationJson,
                  ss.str());
}

// https://stackoverflow.com/a/63864750
std::string NowAsISO8601TimeUTC() {
  auto now = std::chrono::system_clock::now();
  auto itt = std::chrono::system_clock::to_time_t(now);
  std::ostringstream ss;
  ss << std::put_time(gmtime(&itt), "%FT%TZ");
  return ss.str();
}

struct LogRecord {
  std::string time;
  std::string message;
};

static cs::db::Table cs_log_table =
    cs::db::Table<LogRecord>();

Response GetLogs(Request request) {
  auto logs = cs_log_table.query_view().values();
  std::stringstream ss;
  ss << "<h1>Logs</h1>";
  for (const auto& log : logs) {
    ss << "<p>" << log.time << " " << log.message << "</p>";
  }
  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Response CreateLog(Request request) {
  auto now = NowAsISO8601TimeUTC();
  auto message = request.body();

  if (request.headers()["content-type"] ==
      kContentTypeApplicationJson) {
    Object* object;
    uint cursor = 0;
    ASSIGN_OR_RETURN(object,
                     cs::net::json::parsers::ParseObject(
                         message, &cursor));
    std::stringstream ss;
    cs::net::json::SerializeObject(ss, object, 4, 0);
    message = ss.str();
  }

  // Save to database.
  LogRecord record{now, message};
  cs_log_table.Insert(record);

  // Write to console.
  std::cout << NowAsISO8601TimeUTC() << " "
            << request.body() << std::endl;
  return Response(HTTP_200_OK, kContentTypeTextHtml, "");
}

Result RunMyWebApp() {
  WebApp app;
  // Routes.
  OK_OR_RETURN(app.Register("GET", "/", index));
  OK_OR_RETURN(app.Register("GET", "/render/", render));
  OK_OR_RETURN(app.Register("GET", "/json/", json));
  OK_OR_RETURN(app.Register("GET", "/log/", GetLogs));
  OK_OR_RETURN(app.Register("POST", "/log/", CreateLog));
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
