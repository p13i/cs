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
#endif

#include "cs/app/scene_animator.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/pixel.hh"

#define APP_FRAME_RATE_FPS 1
#define APP_ANIMATION_DURATION_SEC 1
#define APP_ANIMATION_NUM_FRAMES \
  (APP_FRAME_RATE_FPS * APP_ANIMATION_DURATION_SEC)
#define APP_SCREEN_WIDTH 512
#define APP_SCREEN_HEIGHT 512

using ::cs::app::SceneAnimator;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;

int main(int argc, char** argv) {
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
  SceneAnimator animator(APP_ANIMATION_NUM_FRAMES,
                         film_dimensions);

  std::vector<Film> frames;

  std::cout << "Rendering " << APP_ANIMATION_NUM_FRAMES
            << " frames with resolution <"
            << std::get<0>(film_dimensions) << ", "
            << std::get<1>(film_dimensions) << ">... "
            << std::endl;

#ifdef __EMSCRIPTEN__
  emscripten_sleep(0);
#endif  // __EMSCRIPTEN__

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
#ifdef __EMSCRIPTEN__
              emscripten_sleep(0);
#endif  // __EMSCRIPTEN__
            });
      });

  std::cout << "done in " << render_time_ms << " ms!"
            << std::endl;

  size_t frame_i = 0;
  while (true) {
    if (SDL_MUSTLOCK(screen)) {
      SDL_LockSurface(screen);
    }

    // Copy each pixel from the current animation frame
    Film film = frames[frame_i];
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

    // Loop the animation
    frame_i = (frame_i + 1) % APP_ANIMATION_NUM_FRAMES;
  }

  SDL_Quit();

  return 0;
}
