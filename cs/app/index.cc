// Copyright 2011 The Emscripten Authors.  All rights
// reserved. Emscripten is available under two separate
// licenses, the MIT license and the University of
// Illinois/NCSA Open Source License.  Both these licenses
// can be found in the LICENSE file.

#include <SDL/SDL.h>
#include <stdint.h>
#include <stdio.h>

#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "cs/app/scene_animator.hh"

#define APP_FRAME_RATE_FPS 24
#define APP_ANIMATION_NUM_FRAMES APP_FRAME_RATE_FPS * 3
#define APP_SCREEN_WIDTH 256
#define APP_SCREEN_HEIGHT 256

using ::cs::app::SceneAnimator;

int main(int argc, char** argv) {
  printf("hello, world!\n");

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

  SceneAnimator animator(
      APP_ANIMATION_NUM_FRAMES,
      {APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT});

  auto frames = animator.render_all_frames();

  size_t iter = 0;
  const size_t max_iterations = APP_ANIMATION_NUM_FRAMES;
  while (true) {
    printf("Iter #%zu of %zu\n", iter, max_iterations);
    if (SDL_MUSTLOCK(screen)) {
      SDL_LockSurface(screen);
    }

    auto film = frames[iter];
    // Copy each pixel
    for (uint32_t i = 0; i < film.width; i++) {
      for (uint32_t j = 0; j < film.height; j++) {
        auto pixel = film.pixels[i][j];
        *((Uint32*)screen->pixels + j * film.width + i) =
            SDL_MapRGBA(screen->format, pixel.r, pixel.g,
                        pixel.b, pixel.a);
      }
    }
    if (SDL_MUSTLOCK(screen)) {
      SDL_UnlockSurface(screen);
    }
    SDL_Flip(screen);
    iter = (iter + 1) % max_iterations;
#ifdef __EMSCRIPTEN__
    emscripten_sleep(1000 / APP_FRAME_RATE_FPS);
#endif  // __EMSCRIPTEN__
  }

  SDL_Quit();

  return 0;
}
