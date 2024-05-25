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
using ::cs::db::Table;
using ::cs::net::http::HTTP_200_OK;
using ::cs::net::http::HTTP_400_BAD_REQUEST;
using ::cs::net::http::HTTP_404_NOT_FOUND;
using ::cs::net::http::kContentTypeApplicationJson;
using ::cs::net::http::kContentTypeTextHtml;
using ::cs::net::http::Request;
using ::cs::net::http::Response;
using ::cs::net::http::WebApp;
using ::cs::net::json::Object;
using ::cs::net::json::parsers::ParseFloat;
using ::cs::net::json::parsers::ParseObject;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;
using ::cs::result::Error;
using ::cs::result::Ok;
using ::cs::result::Result;

namespace {

static WebApp app;

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

static Table cs_log_table = Table<LogRecord>();

struct AppLogger {
  // Overload << operator for ostream
  template <typename T>
  void operator<<(const T& t) {
    // Write to console and the logs table.
    std::cout << t << std::endl;
    std::stringstream ss;
    ss << t << std::endl;
    cs_log_table.insert({NowAsISO8601TimeUTC(), ss.str()});
  }
};

static AppLogger app_log;

}  // namespace

Response index(Request request) {
  app_log << request;

  std::stringstream ss;
  ss << "<h1>Welcome to my website!</h1>";
  ss << "<p>To code this app, I only used standard library "
        "methods (like <code>std::chrono</code> or <code>std::vector</code>) for "
        "this web app framework.</p>";
  ss << "<h2>Routes</h2>";
  ss << "<ul>";
  for (const auto& [method, path] : app.Routes()) {
    ss << "<li><code>" << method << " <a href=\"" << path
       << "\">" << path << "</a></code></li>";
  }
  ss << "</ul>";
  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Response RenderOnServer(Request request) {
  app_log << request;

  float width_f;
  ASSIGN_OR_RETURN(
      width_f,
      ParseFloat(request.get_query_param("width").value_or(
          std::to_string(APP_SCREEN_WIDTH))));

  uint width = static_cast<uint>(width_f);
  if (width < 2) {
    return Error("Width must be at least 2 pixels.");
  }

  float height_f;
  ASSIGN_OR_RETURN(
      height_f,
      ParseFloat(request.get_query_param("height").value_or(
          std::to_string(APP_SCREEN_HEIGHT))));

  uint height = static_cast<uint>(height_f);
  if (height < 2) {
    return Error("Height must be at least 2 pixels.");
  }

  float num_frames_f;
  ASSIGN_OR_RETURN(
      num_frames_f,
      ParseFloat(request.get_query_param("num_frames")
                     .value_or(std::to_string(1))));

  uint num_frames = static_cast<uint>(num_frames_f);
  if (num_frames < 1) {
    return Error("Number of frames must be at least 1.");
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
        std::vector<float> rgba = {
            static_cast<float>(px.r),
            static_cast<float>(px.g),
            static_cast<float>(px.b),
            static_cast<float>(px.a)};
        Object* pixel_json = new Object(rgba);
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
  float indent;
  ASSIGN_OR_RETURN(
      indent,
      ParseFloat(request.get_query_param("indent").value_or(
          std::to_string(0))));

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
  cs::net::json::SerializeObjectPrettyPrintRecurse(
      ss, object, static_cast<uint>(indent), 0);
  delete object;
  return Response(HTTP_200_OK, kContentTypeApplicationJson,
                  ss.str());
}

Response GetLogs(Request request) {
  app_log << request;

  auto logs = cs_log_table.query_view().values();
  std::stringstream ss;
  ss << "<h1>Logs</h1>";
  for (const auto& log : logs) {
    ss << "<li><code>" << log.time << " " << log.message
       << "</code></li>";
  }
  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Response CreateLog(Request request) {
  app_log << request;

  std::string now = NowAsISO8601TimeUTC();

  std::string message;
  if (request.headers()["content-type"] ==
      kContentTypeApplicationJson) {
    Object* object;
    uint cursor = 0;
    ASSIGN_OR_RETURN(object, ParseObject(message, &cursor));
    std::stringstream ss;
    cs::net::json::SerializeObjectPrettyPrintRecurse(
        ss, object, /*indent=*/4, 0);
    message = ss.str();
  } else {
    message = request.body();
  }

  // Save to database.
  cs_log_table.insert({now, message});

  // Write to console.
  std::cout << NowAsISO8601TimeUTC() << " "
            << request.body() << std::endl;
  return Response(HTTP_200_OK, kContentTypeTextHtml, "");
}

Response render_in_browser(Request request) {
  std::stringstream ss;
  ss << R"html(<!doctype html>
<html lang="en-us">
<head>
  <meta charset="utf-8">
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title>C++ Graphics Simulators in Browser
  </title>
</head>
<body>
  <h1>Raytracer Demo</h1>
  <hr/>
  <div id="spinner">
  </div>
  <div id="status">
    Downloading...</div>
  <span id="controls">
    <span><input type="checkbox" id="resize">Resize canvas</span>
    <span><input type="checkbox" id="pointerLock" checked>Lock/hide mouse
      pointer &nbsp;&nbsp;&nbsp;</span>
    <span><input type="button" value="Fullscreen"
        onclick="Module.requestFullscreen(
          document.getElementById('pointerLock').checked,
          document.getElementById('resize').checked)">
    </span>
  </span>
  <progress value="0" max="100" id="progress" hidden=1></progress>
  <hr/>
  <canvas id="canvas"
    oncontextmenu="event.preventDefault()" tabindex=-1></canvas>
  <hr/>
  <textarea id="output" rows="40" cols="80"></textarea>
  <script type="text/javascript" src="https://p13i.io/cs/wasm.js"></script>
  <script async type="text/javascript" src="https://p13i.io/cs/index.js"></script>
</body>
</html>)html";
  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Response Render(const Request& request) {
  std::stringstream ss;
  ss << "<h1>Ray-tracer</h1>";
  ss << "Form: scene name, height, width, number frames, "
        "checkboxes for server and browser-side rendering.";
  return Response(HTTP_200_OK, kContentTypeTextHtml,
                  ss.str());
}

Result RunMyWebApp() {
  // Routes.
  OK_OR_RETURN(app.Register("GET", "/", index));
  OK_OR_RETURN(app.Register("GET", "/render/", Render));
  OK_OR_RETURN(app.Register("GET", "/render-on-server/",
                            RenderOnServer));
  OK_OR_RETURN(app.Register("GET", "/render-in-browser/",
                            render_in_browser));
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
