#ifndef CS_LINALG_TRANSFORM_HH
#define CS_LINALG_TRANSFORM_HH

// Much of this file is based on:
// https://pbr-book.org/3ed-2018/Geometry_and_Transformations/Transformations

#include "cs/geo/cross.hh"
#include "cs/geo/vector3.h"
#include "cs/linalg/matrix4x4.hh"
#include "cs/result/result.hh"

namespace cs::linalg {

using m4x4 = ::cs::linalg::Matrix4x4;
using v3 = ::cs::geo::Vector3;
using ::cs::geo::cross;
using ::cs::result::Error;
using ::cs::result::ResultOr;

struct Transform {
 private:
  m4x4 m_;
  m4x4 m_inv_;

 public:
  // Default transform is identity matrix
  explicit Transform()
      : Transform(Matrix4x4(), Matrix4x4()) {}
  Transform(const m4x4& m)
      : Transform(m, m.inverse().value()) {}
  Transform(m4x4 m, m4x4 m_inv) : m_(m), m_inv_(m_inv) {}
  Transform inverse() { return Transform(m_inv_, m_); }
  inline ResultOr<p3> operator()(const p3& p) const {
    float x = p.x, y = p.y, z = p.z;
    // clang-format off
    float xp = m_.data_[0][0] * x + m_.data_[0][1] * y + m_.data_[0][2] * z + m_.data_[0][3];
    float yp = m_.data_[1][0] * x + m_.data_[1][1] * y + m_.data_[1][2] * z + m_.data_[1][3];
    float zp = m_.data_[2][0] * x + m_.data_[2][1] * y + m_.data_[2][2] * z + m_.data_[2][3];
    float wp = m_.data_[3][0] * x + m_.data_[3][1] * y + m_.data_[3][2] * z + m_.data_[3][3];
    // clang-format on
    if (FloatsNear(wp, 0)) {
      return Error("Division by zero");
    }
    return p3(xp, yp, zp) / wp;
  }
};

namespace transforms {

Transform Translate(v3 delta);

Transform Scale(float x, float y, float z);

Transform RotateX(float theta_rad);

Transform RotateY(float theta_rad);

Transform RotateZ(float theta_rad);

// Rotation around arbitrary axis. See:
// https://github.com/mmp/pbrt-v3/blob/13d871faae88233b327d04cda24022b8bb0093ee/src/core/transform.cpp#L179-L201
Transform Rotate(float theta_rad, const v3& axis);

Transform LookAt(p3 pos, p3 look, v3 up);

}  // namespace transforms

}  // namespace cs::linalg

#endif  // CS_LINALG_TRANSFORM_HH
