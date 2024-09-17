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
#endif

#include "cs/app/scene1.hh"
#include "cs/app/scene_animator.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"

using ::cs::app::SceneAnimator;
using ::cs::linalg::Transform;
using ::cs::linalg::transforms::Rotate;
using ::cs::linalg::transforms::RotateY;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;

static bool scene_changed = true;
static const float focal_length = 5;
static p3 pos(-25, 0, -10);
static p3 look(0, 0, 0);
static p3 up(0, 1, 0);

#ifdef __EMSCRIPTEN__
// Callback function to handle keydown events
EM_BOOL key_callback(int eventType,
                     const EmscriptenKeyboardEvent* e,
                     void* userData) {
#if 1
  std::cout << std::hex << e->keyCode << std::endl;
#endif
  p3 delta;
  if (e->keyCode == DOM_VK_LEFT) {
    v3 cross = cs::geo::cross(pos - look, up);
    delta = -1 * cross.unit().point();
    scene_changed = true;
  } else if (e->keyCode == DOM_VK_RIGHT) {
    v3 cross = cs::geo::cross(pos - look, up);
    delta = +1 * cross.unit().point();
    scene_changed = true;
  } else if (e->keyCode == DOM_VK_UP) {
    delta = +1 * up.unit();
    scene_changed = true;
  } else if (e->keyCode == DOM_VK_DOWN) {
    delta = -1 * up.unit();
    scene_changed = true;
  } else if (e->keyCode == DOM_VK_EQUALS ||
             e->keyCode == 187) {
    // move towards look
    delta = -1 * (pos - look).unit();
    scene_changed = true;
  } else if (e->keyCode == DOM_VK_HYPHEN_MINUS ||
             e->keyCode == 189) {
    // move away from look
    delta = +1 * (pos - look).unit();
    scene_changed = true;
  } else if (e->keyCode == DOM_VK_OPEN_BRACKET) {
    // Rotate viewer to left.
    auto d = v3(look - pos).normalized().point();
    auto d_prime = RotateY(-1 * 5.f * PIf / 180.f)(d);
    if (d_prime.ok()) {
      look = pos +
             v3(look - pos).magnitude() * d_prime.value();
      scene_changed = true;
    }
  } else if (e->keyCode == DOM_VK_CLOSE_BRACKET) {
    // Rotate viewer to left.
    auto d = v3(look - pos).normalized().point();
    auto d_prime = RotateY(+1 * 5.f * PIf / 180.f)(d);
    if (d_prime.ok()) {
      look = pos +
             v3(look - pos).magnitude() * d_prime.value();
      scene_changed = true;
    }
  }
  pos += delta;
  look += delta;
  // Prevent default browser behavior.
  return EM_TRUE;
}
#endif  // __EMSCRIPTEN__

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

  std::tuple<unsigned int, unsigned int> film_dimensions(
      APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);

  // Setup scene
  std::vector<Shape*> shapes{
      // Unit sphere at the origin
      new Sphere(p3(0, 0, 0), 0.5),
      // One smaller sphere at x=-1
      new Sphere(p3(-1, 0, 0), 0.25),
      // One even smaller sphere at y=1
      new Sphere(p3(0, 1, 0), 0.125),
      // One even smaller sphere at z=-1
      new Sphere(p3(0, 0, -1), 0.0625),
      // new Sphere(p3(2, 0, 0), 0.5),
      // new Sphere(p3(0, 2, 0), 0.25),
      // new Sphere(p3(0, -1, 0), 0.25),
      // new Plane(p3(1, 1, 1).unit(), -5),
  };

  Scene scene(shapes);

  // Setup camera
  unsigned int pixels_per_unit =
      std::min(std::get<0>(film_dimensions),
               std::get<1>(film_dimensions)) /
      2;

  while (true) {
    while (!scene_changed) {
#ifdef __EMSCRIPTEN__
      // Release main thread
      emscripten_sleep(0);
#endif  // __EMSCRIPTEN__
    }
    scene_changed = false;

    if (SDL_MUSTLOCK(screen)) {
      SDL_LockSurface(screen);
    }

    // Copy each pixel from the current animation frame
    Transform w2c = LookAt(pos, look, up);
    Camera camera(w2c, pixels_per_unit, focal_length,
                  Film(film_dimensions));

    // Setup renderer
    SceneRenderer renderer(camera, scene);
    Film film = renderer.render();
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

#if 0
#ifdef __EMSCRIPTEN__
    emscripten_sleep(1000 / APP_FRAME_RATE_FPS);
#endif  // __EMSCRIPTEN__
#endif  // 0
  }

  SDL_Quit();

#ifdef __EMSCRIPTEN__
  // Keep the program running
  emscripten_exit_with_live_runtime();
#endif  // __EMSCRIPTEN__

  return 0;
}
