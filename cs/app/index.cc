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
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;

static p3 pos(-25, 0, -10);
static p3 look(0, 0, 0);

#ifdef __EMSCRIPTEN__
// Callback function to handle keydown events
EM_BOOL key_callback(int eventType,
                     const EmscriptenKeyboardEvent* e,
                     void* userData) {
  printf("Key pressed: %s\n",
         e->key);  // Print the key pressed
  p3 new_pos = pos;
  if (e->keyCode == DOM_VK_LEFT) {
    new_pos.x -= 1;
  } else if (e->keyCode == DOM_VK_RIGHT) {
    new_pos.x += 1;
  } else if (e->keyCode == DOM_VK_UP) {
    new_pos.y += 1;
  } else if (e->keyCode == DOM_VK_DOWN) {
    new_pos.y -= 1;
  } else if (e->keyCode == DOM_VK_EQUALS) {
    // move towards look
    new_pos = (pos - look) * 0.8;
  } else if (e->keyCode == DOM_VK_HYPHEN_MINUS) {
    // move away from look
    new_pos = (pos - look) * 1.2;
  }
  pos = new_pos;
  return EM_TRUE;  // Return EM_TRUE to prevent the default
                   // browser behavior
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

  std::cout << "Rendering " << APP_ANIMATION_NUM_FRAMES
            << " frames with resolution <"
            << std::get<0>(film_dimensions) << ", "
            << std::get<1>(film_dimensions) << ">... "
            << "\n";

#ifdef __EMSCRIPTEN__
  emscripten_sleep(0);
#endif  // __EMSCRIPTEN__

  while (true) {
    if (SDL_MUSTLOCK(screen)) {
      SDL_LockSurface(screen);
    }

    // Copy each pixel from the current animation frame
    p3 up(0, 1, 0);
    Transform w2c = LookAt(pos, look, up);
    float focal_length = 5;
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

#ifdef __EMSCRIPTEN__
    emscripten_sleep(1000 / APP_FRAME_RATE_FPS);
#endif  // __EMSCRIPTEN__
  }

  SDL_Quit();

#ifdef __EMSCRIPTEN__
  // Keep the program running
  emscripten_exit_with_live_runtime();
#endif  // __EMSCRIPTEN__

  return 0;
}
