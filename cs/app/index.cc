// Copyright 2011 The Emscripten Authors.  All rights
// reserved. Emscripten is available under two separate
// licenses, the MIT license and the University of
// Illinois/NCSA Open Source License.  Both these licenses
// can be found in the LICENSE file.

#include <SDL/SDL.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define APP_FRAME_RATE_FPS 24

int main(int argc, char** argv) {
  printf("hello, world!\n");

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* screen =
      SDL_SetVideoMode(256, 256, 32, SDL_SWSURFACE);

#ifdef TEST_SDL_LOCK_OPTS
  EM_ASM(
      "SDL.defaults.copyOnLock = false; "
      "SDL.defaults.discardOnLock = true; "
      "SDL.defaults.opaqueFrontBuffer = false;");
#endif

  size_t iter = 0;
  const size_t max_iterations = 256;
  while (iter < max_iterations) {
    printf("Iter #%zu\n", iter);
    if (SDL_MUSTLOCK(screen)) {
      SDL_LockSurface(screen);
    }
    for (int i = 0; i < 256; i++) {
      for (int j = 0; j < 256; j++) {
#ifdef TEST_SDL_LOCK_OPTS
        // Alpha behaves like in the browser, so write
        // proper opaque pixels.
        int alpha = 255;
#else
        // To emulate native behavior with blitting to
        // screen, alpha component is ignored. Test that it
        // is so by outputting data (and testing that it
        // does get discarded)
        int alpha = (i + j) % 255;
#endif
        *((Uint32*)screen->pixels + i * 256 + j) =
            SDL_MapRGBA(screen->format, iter, j, 255 - i,
                        alpha);
      }
    }
    if (SDL_MUSTLOCK(screen)) {
      SDL_UnlockSurface(screen);
    }
    SDL_Flip(screen);
    iter++;
#if 0
    for (int t = 0; t < 100000; t++) {
      // sleep...
      printf("%d, ", t);
    }
#else
    emscripten_sleep(1000 / APP_FRAME_RATE_FPS);
#endif
  }

  SDL_Quit();

  return 0;
}
