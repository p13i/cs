#ifndef CS_MATH_MATRIX4X4_HH
#define CS_MATH_MATRIX4X4_HH

#include <stdint.h>

#include "cs/string/format.h"

using ::cs::string::format;

namespace cs::math {
struct Matrix4x4 {
 private:
  float data_[4][4];

 public:

  // Default matrix is identity matrix
  // clang-format off
  Matrix4x4() : Matrix4x4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f) {}
  Matrix4x4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33): Matrix4x4(
        new float[4][4]{{m00, m01, m02, m03},
                        {m10, m11, m12, m13},
                        {m20, m21, m22, m23},
                        {m30, m31, m32, m33}}) {
  }
  // clang-format on

  Matrix4x4(float data[4][4]) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        data_[i][j] = data[i][j];
      }
    }
  }
  // Gets the value at x, y. x indexes into columns, y into
  // the rows
  float get(uint8_t x, uint8_t y) const;
  // Sets the value at x, y.
  void set(uint8_t x, uint8_t y, float datum);

  // inline Point3<T> operator()(const Point3<T> &p) const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const Matrix4x4 &m) {
    // clang-format off
    return os << format(R"([ [ %f, %f, %f, %f ]
[ %f, %f, %f, %f ]
[ %f, %f, %f, %f ]
[ %f, %f, %f, %f ] ])",
      m.get(0, 0), m.get(0, 1), m.get(0, 2), m.get(0, 3),
      m.get(1, 0), m.get(1, 1), m.get(1, 2), m.get(1, 3),
      m.get(2, 0), m.get(2, 1), m.get(2, 2), m.get(2, 3),
      m.get(3, 0), m.get(3, 1), m.get(3, 2), m.get(3, 3));;
    // clang-format on
  }
};
}  // namespace cs::math

#endif  // CS_MATH_MATRIX4X4_HH