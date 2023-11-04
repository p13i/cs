#include "cs/renderer/scene_renderer.hh"

#include "cs/renderer/film.hh"
#include "cs/renderer/scene.hh"
#include "cs/shapes/plane.hh"
#include "cs/shapes/sphere.hh"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::cs::linalg::transforms::LookAt;
using ::cs::renderer::Camera;
using ::cs::renderer::Film;
using ::cs::renderer::Pixel;
using ::cs::renderer::Scene;
using ::cs::renderer::SceneRenderer;
using ::cs::shapes::Plane;
using ::cs::shapes::Sphere;

TEST(SceneRenderer, 2x2) {
  // Setup camera
  p3 film_center(0, 0, -1);
  int pixels_per_unit = 1;
  Transform w2c =
      LookAt(p3(-1, 0, 0), p3(0, 0, 0), p3(0, 1, 0));
  float focal_length = -2;
  Camera camera(w2c, pixels_per_unit, focal_length,
                Film(std::tuple<unsigned int, unsigned int>(
                    256, 256)));

  // Setup scene
  std::vector<Shape*> shapes;
  // Sphere right at origin
  shapes.push_back(new Sphere(/*center=*/p3(0, 0, 0),
                              /*radius=*/0.1));
  // Plane on z-axis +10 units away from origin
  shapes.push_back(new Plane(p3(0, 0, 1).unit(), -10));

  Scene scene(shapes);

  // Setup renderer
  SceneRenderer renderer(camera, scene);
  Film film = renderer.render();

  Pixel** pixels = film.pixels;
  EXPECT_EQ(pixels[128][128], Pixel(255, 255, 255, 255));
}
