// Copyright 2011 The Emscripten Authors.  All rights
// reserved. Emscripten is available under two separate
// licenses, the MIT license and the University of
// Illinois/NCSA Open Source License.  Both these licenses
// can be found in the LICENSE file.

#include <SDL/SDL.h>
#include <stdint.h>
#include <stdio.h>

#include <sstream>
#include <tuple>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/key_codes.h>
#endif  // __EMSCRIPTEN__

#include "cs/app/scene1.hh"
#include "cs/app/scene_animator.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"

using ::cs::app::SceneAnimator;
using ::cs::app::text::fonts::SampleCharacterPixel;
using ::cs::linalg::Transform;
using ::cs::linalg::transforms::Rotate;
using ::cs::linalg::transforms::RotateY;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;

static bool scene_changed = true;
static const float focal_length = 5;
static p3 pos(0, 0, -10);
static p3 look(0, 0, 0);
static p3 up(0, 1, 0);
static Scene scene({
    // Unit sphere at the origin.
    new Sphere(p3(0, 0, 0), 1),
    // Walls on left and right.
    new Plane(p3(+1, 0, 0).unit(), 10),
    new Plane(p3(-1, 0, 0).unit(), 10),
    // Walls on ceiling and floor.
    new Plane(p3(0, +1, 0).unit(), 5),
    new Plane(p3(0, -1, 0).unit(), 5),
    // Walls in front.
    new Plane(p3(0, 0, 1).unit(), -30),
    // new Plane(p3(+1, 0, 0).unit(), +5),
});
static std::string message = "GAME START";

#ifdef __EMSCRIPTEN__
// Callback function to handle keydown events
EM_BOOL key_callback(int eventType,
                     const EmscriptenKeyboardEvent* e,
                     void* userData) {
  if (e->keyCode == DOM_VK_LEFT) {
    v3 cross = cs::geo::cross(pos - look, up);
    p3 delta = -1 * cross.unit().point();
    pos += delta;
    look += delta;
    scene_changed = true;
    message = "MOVED LEFT";
  } else if (e->keyCode == DOM_VK_RIGHT) {
    v3 cross = cs::geo::cross(pos - look, up);
    p3 delta = +1 * cross.unit().point();
    pos += delta;
    look += delta;
    scene_changed = true;
    message = "MOVED RIGHT";
  } else if (e->keyCode == DOM_VK_UP) {
    p3 delta = +1 * up.unit();
    pos += delta;
    look += delta;
    scene_changed = true;
    message = "MOVED UP";
  } else if (e->keyCode == DOM_VK_DOWN) {
    p3 delta = -1 * up.unit();
    pos += delta;
    look += delta;
    scene_changed = true;
    message = "MOVED DOWN";
  } else if (e->keyCode == DOM_VK_EQUALS ||
             e->keyCode == 187) {
    // move towards look
    p3 delta = -1 * (pos - look).unit();
    pos += delta;
    look += delta;
    scene_changed = true;
    message = "MOVED FORWARD";
  } else if (e->keyCode == DOM_VK_HYPHEN_MINUS ||
             e->keyCode == 189) {
    // move away from look
    p3 delta = +1 * (pos - look).unit();
    pos += delta;
    look += delta;
    scene_changed = true;
    message = "MOVED BACK";
  } else if (e->keyCode == DOM_VK_OPEN_BRACKET) {
    // Rotate viewer to left.
    auto d = v3(look - pos).normalized().point();
    auto d_prime = RotateY(-1 * 5.f * PIf / 180.f)(d);
    if (d_prime.ok()) {
      look = pos +
             v3(look - pos).magnitude() * d_prime.value();
      scene_changed = true;
      message = "ROTATE LEFT";
    }
  } else if (e->keyCode == DOM_VK_CLOSE_BRACKET) {
    // Rotate viewer to right.
    auto d = v3(look - pos).normalized().point();
    auto d_prime = RotateY(+1 * 5.f * PIf / 180.f)(d);
    if (d_prime.ok()) {
      look = pos +
             v3(look - pos).magnitude() * d_prime.value();
      scene_changed = true;
      message = "ROTATE RIGHT";
    }
  }
  // Prevent default browser behavior if the scene changed.
  return scene_changed ? EM_TRUE : EM_FALSE;
}
#endif  // __EMSCRIPTEN__

void DrawString(Film* film, int* xStart, const int yStart,
                std::string str, int margin = 1) {
  for (char ch : str) {
    *xStart += margin;
    for (uint x = 0; x < 8; x++) {
      for (uint y = 0; y < 8; y++) {
        bool value = SampleCharacterPixel(ch, x, y);
        char rgba = value ? 255 : 0;
        uint film_x = *xStart + x;
        uint film_y = yStart + margin + y;
        if (film_x < 0 || film_x >= film->width ||
            film_y < 0 || film_y >= film->height ||
            rgba == 0) {
          continue;
        }
        film->pixels[film_x][film_y] =
            cs::renderer::Pixel(rgba, rgba, rgba, rgba);
      }
    }
    // Move the x start position
    *xStart += margin + 8;
  }
}

int main(int argc, char** argv) {
#ifdef __EMSCRIPTEN__
  // Set up the keydown event listener
  emscripten_set_keydown_callback(
      EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, EM_TRUE,
      key_callback);
#endif  // __EMSCRIPTEN__

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen =
      SDL_SetVideoMode(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT,
                       32, SDL_SWSURFACE);

#ifdef TEST_SDL_LOCK_OPTS
  EM_ASM(
      "SDL.defaults.copyOnLock = false; "
      "SDL.defaults.discardOnLock = true; "
      "SDL.defaults.opaqueFrontBuffer = false;");
#endif

  // Setup camera
#if 0
  uint pixels_per_unit =
      std::min(APP_SCREEN_WIDTH,
               APP_SCREEN_HEIGHT) /
      2;
#else
  uint pixels_per_unit = 10;
#endif

  std::cout << R"(Controls:
Arrow keys:
- up: move up
- down: move down
- left: move left
- right: move right
Open bracket (`[`): rotate left
Close bracket (`]`): rotate right
Minus (`-`/`_`): move backwards
Plus (`=`/`+`): move forwards
)";

  Film film(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);

  while (true) {
    while (!scene_changed) {
#ifdef __EMSCRIPTEN__
      // Release main thread for keyboard interrupts.
      emscripten_sleep(0);
#endif  // __EMSCRIPTEN__
    }
    scene_changed = false;

    if (SDL_MUSTLOCK(screen)) {
      SDL_LockSurface(screen);
    }

    // Copy each pixel from the current animation frame
    Transform w2c = LookAt(pos, look, up);
    Camera camera(w2c, pixels_per_unit, focal_length, film);

    // Setup renderer
    SceneRenderer renderer(camera, scene);
    renderer.render();

    // Draw message.
    int xStart = 10;
    int yStart = 10;
    DrawString(&film, &xStart, yStart, message);

    // Draw alphabet near bottom
    xStart = 10;
    yStart = APP_SCREEN_HEIGHT - 10 * 2;
    DrawString(&film, &xStart, yStart,
               "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    for (uint32_t i = 0; i < film.width; i++) {
      for (uint32_t j = 0; j < film.height; j++) {
        Pixel pixel = film.pixels[i][j];
        *((Uint32*)screen->pixels + j * film.width + i) =
            SDL_MapRGBA(screen->format, pixel.r, pixel.g,
                        pixel.b, pixel.a);
      }
    }

    if (SDL_MUSTLOCK(screen)) {
      SDL_UnlockSurface(screen);
    }

    // Display the screen
    SDL_Flip(screen);

#ifdef __EMSCRIPTEN__
#if 1
    emscripten_sleep(1000 / APP_FRAME_RATE_FPS);
#else
    emscripten_sleep(0);
#endif
#endif  // __EMSCRIPTEN__
  }

  SDL_Quit();

#ifdef __EMSCRIPTEN__
  // Keep the program running
  emscripten_exit_with_live_runtime();
#endif  // __EMSCRIPTEN__

  return 0;
}
