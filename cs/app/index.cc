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

#include "cs/numbers/map_value.hh"
#include "cs/renderer/rainbow.h"
#include "cs/renderer/scene_renderer.hh"
#include "cs/renderer/sphere_renderer.hh"

#define APP_FRAME_RATE_FPS 24
#define APP_SCREEN_WIDTH 256
#define APP_SCREEN_HEIGHT 256

using ::cs::numbers::map_value;

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

#if 0
  cs::renderer::RainbowRenderer renderer;
  cs::renderer::Film film(APP_SCREEN_WIDTH,
                          APP_SCREEN_HEIGHT);
  renderer.render(&film);
#elif 0
  cs::renderer::SphereRenderer renderer;
  cs::renderer::Film film = renderer.render();
#else
#endif

  cs::renderer::Film film;
  size_t iter = 0;
  const size_t max_iterations = APP_FRAME_RATE_FPS * 5;
  while (iter < max_iterations) {
    printf("Iter #%zu of %zu\n", iter, max_iterations);
    if (SDL_MUSTLOCK(screen)) {
      SDL_LockSurface(screen);
    }
    float focal_point_z = map_value<float>(
        iter, 0, max_iterations, -10, -1.5);
    cs::renderer::SceneRenderer renderer(
        p3(0, 0, focal_point_z));
    film = renderer.render();
    for (uint32_t i = 0; i < film.width; i++) {
      for (uint32_t j = 0; j < film.height; j++) {
        cs::renderer::Pixel pixel = film.pixels[i][j];
        *((Uint32*)screen->pixels + j * film.width + i) =
            SDL_MapRGBA(screen->format, pixel.r, pixel.g,
                        pixel.b, pixel.a);
      }
    }
    if (SDL_MUSTLOCK(screen)) {
      SDL_UnlockSurface(screen);
    }
    SDL_Flip(screen);
    iter++;
#ifdef __EMSCRIPTEN__
    emscripten_sleep(1000 / APP_FRAME_RATE_FPS);
#endif  // __EMSCRIPTEN__
  }

  SDL_Quit();

  return 0;
}
