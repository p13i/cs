#include "cs/renderer/rainbow.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::renderer::Film;
using ::cs::renderer::Pixel;
using ::cs::renderer::RainbowRenderer;

TEST(RainbowRenderer, 1x1) {
  Film film(1, 1);
  RainbowRenderer renderer;
  renderer.render(&film);

  Pixel** pixels = film.pixels;
  EXPECT_EQ(pixels[0][0], Pixel(128, 128, 128, 255));
}

TEST(RainbowRenderer, 256x256) {
  Film film(256, 256);
  RainbowRenderer renderer;
  renderer.render(&film);

  Pixel** pixels = film.pixels;
  EXPECT_EQ(pixels[0][0], Pixel(0, 0, 255, 255));
  EXPECT_EQ(pixels[0][127], Pixel(0, 127, 255, 255));
  EXPECT_EQ(pixels[127][0], Pixel(127, 0, 128, 255));
  EXPECT_EQ(pixels[127][127], Pixel(127, 127, 128, 255));
  EXPECT_EQ(pixels[255][255], Pixel(255, 255, 0, 255));
}
