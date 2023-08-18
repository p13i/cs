#ifndef CS_APP_SCENE_ANIMATOR_HH
#define CS_APP_SCENE_ANIMATOR_HH

#include <stdio.h>

#include <functional>
#include <vector>

#include "cs/collections/tuple.hh"
#include "cs/profiling/time_it.hh"
#include "cs/renderer/film.hh"
#include "cs/renderer/scene.hh"
#include "cs/renderer/scene_renderer.hh"

using ::cs::collections::Tuple;
using ::cs::profiling::time_it;
using ::cs::renderer::Camera;
using ::cs::renderer::Film;
using ::cs::renderer::Scene;
using ::cs::renderer::SceneRenderer;
using p3 = ::cs::geo::Point3;
using v3 = ::cs::geo::Vector3;
using r3 = ::cs::geo::Ray3;

namespace cs::app {

struct SceneAnimator {
  size_t num_frames_;
  Tuple<unsigned int, unsigned int> film_dimensions_;
  SceneAnimator(
      size_t num_frames,
      Tuple<unsigned int, unsigned int> film_dimensions)
      : num_frames_(num_frames),
        film_dimensions_(film_dimensions) {}

  std::vector<Film> render_all_frames() {
    std::vector<Film> frames(num_frames_);

    for (size_t i = 0; i < num_frames_; i++) {
#if 0
      std::cout << "Computing frame #" << i << " of "
                << num_frames_ << "... ";

      // Animate focal point or film center
      float focal_point_z =
          map_value<float>(i, 0, num_frames_, -40, 1);
      p3 dynamic_focal_point(0, 0, focal_point_z);
      p3 film_center(0, 0, 0);
#else
      p3 film_center(
          map_value<p3>(p3(i), p3(0), p3(num_frames_),
                        p3(0, 0, -1), p3(0, 0, -10)));
      p3 dynamic_focal_point = film_center - p3(0, 0, 2);
#endif

      // Setup camera
      Camera camera(dynamic_focal_point, film_center,
                    std::min(film_dimensions_.first(),
                             film_dimensions_.second()) /
                        2,
                    film_dimensions_);

#if 0
      std::cout << "dynamic_focal_point="
                << dynamic_focal_point
                << ", film_center=" << film_center
                << std::endl;
#endif

      // Setup scene
      Scene scene({Sphere(/*center=*/p3(0, 0, 0),
                          /*radius=*/1),
                   Sphere(/*center=*/p3(2, 0, 0),
                          /*radius=*/0.5),
                   Sphere(/*center=*/p3(0, 2, 0),
                          /*radius=*/0.25),
                   Sphere(/*center=*/p3(0, -1, 0),
                          /*radius=*/0.25),
                   Sphere(/*center=*/p3(0, 0, -1),
                          /*radius=*/0.25)});
      // Setup renderer
      SceneRenderer renderer(camera, scene);

      Film film;
      // Measure the render time
      [[maybe_unused]] unsigned int render_time_ms =
          time_it([&film, &renderer]() {
            film = renderer.render();
          });
      frames[i] = film;

#if 0
      std::cout << "Render time (ms): " << render_time_ms
                << std::endl;
#endif
    }

    return frames;
  }
};

}  // namespace cs::app

#endif  // CS_APP_SCENE_ANIMATOR_HH