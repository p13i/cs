#ifndef CS_LINALG_TRANSFORM_HH
#define CS_LINALG_TRANSFORM_HH

// Much of this file is based on:
// https://pbr-book.org/3ed-2018/Geometry_and_Transformations/Transformations

#include "cs/geo/cross.hh"
#include "cs/geo/vector3.h"
#include "cs/linalg/matrix4x4.hh"
#include "cs/sanity/ensure.hh"

namespace cs::linalg {

using m4x4 = ::cs::linalg::Matrix4x4;
using v3 = ::cs::geo::Vector3;
using ::cs::geo::cross;

struct Transform {
 private:
  m4x4 m_;
  m4x4 m_inv_;

 public:
  Transform(const m4x4& m) : Transform(m, m.inverse()) {}
  Transform(m4x4 m, m4x4 m_inv) : m_(m), m_inv_(m_inv) {}
  Transform inverse() { return Transform(m_inv_, m_); }
  inline p3 Transform::operator()(const p3& p) const {
    float x = p.x, y = p.y, z = p.z;
    // clang-format off
    float xp = m_.data_[0][0] * x + m_.data_[0][1] * y + m_.data_[0][2] * z + m_.data_[0][3];
    float yp = m_.data_[1][0] * x + m_.data_[1][1] * y + m_.data_[1][2] * z + m_.data_[1][3];
    float zp = m_.data_[2][0] * x + m_.data_[2][1] * y + m_.data_[2][2] * z + m_.data_[2][3];
    float wp = m_.data_[3][0] * x + m_.data_[3][1] * y + m_.data_[3][2] * z + m_.data_[3][3];
    // clang-format on
    ENSURE(wp != 0);
    if (wp == 1) {
      return p3(xp, yp, zp);
    } else {
      return p3(xp, yp, zp) / wp;
    }
  }
};

namespace transforms {

Transform Translate(v3 delta) {
  // clang-format off
  m4x4 m(
    1,0,0,delta.x,
    0,1,0,delta.y,
    0,0,1,delta.z,
    0,0,0,1);
  m4x4 m_inv(
    1,0,0,-delta.x,
    0,1,0,-delta.y,
    0,0,1,-delta.z,
    0,0,0,1);
  // clang-format on
  return Transform(m, m_inv);
}

Transform Scale(float x, float y, float z) {
  // clang-format off
  m4x4 m(
    x,0,0,0,
    0,y,0,0,
    0,0,z,0,
    0,0,0,1);
  m4x4 m_inv(
    1/x, 0,   0,   0,
    0,   1/y, 0,   0,
    0,   0,   1/z, 0,
    0,   0,   0,   1);
  // clang-format on
  return Transform(m, m_inv);
}

Transform RotateX(float theta_rad) {
  float sinTheta = std::sin(theta_rad);
  float cosTheta = std::cos(theta_rad);
  // clang-format off
  Matrix4x4 m(1,        0,         0, 0, 
              0, cosTheta, -sinTheta, 0,
              0, sinTheta,  cosTheta, 0,
              0,        0,         0, 1);
  // clang-format on
  return Transform(m, m.T());
}

Transform RotateY(float theta_rad) {
  float sinTheta = std::sin(theta_rad);
  float cosTheta = std::cos(theta_rad);
  // clang-format off
  Matrix4x4 m(
    cosTheta,   0,  sinTheta, 0, 
    0,          1,  0,        0,
    -sinTheta,  0,  cosTheta, 0,
    0,          0,  0,        1);
  // clang-format on
  return Transform(m, m.T());
}

Transform RotateZ(float theta_rad) {
  float sinTheta = std::sin(theta_rad);
  float cosTheta = std::cos(theta_rad);
  // clang-format off
  Matrix4x4 m(
    cosTheta, -sinTheta,  0, 0, 
    sinTheta, cosTheta,   0, 0,
    0,        0,          1, 0,
    0,        0,          0, 1);
  // clang-format on
  return Transform(m, m.T());
}

Transform LookAt(p3 pos, p3 look, v3 up) {
  m4x4 cameraToWorld;
  // <<Initialize fourth column of viewing matrix>>
  cameraToWorld.data_[0][3] = pos.x;
  cameraToWorld.data_[1][3] = pos.y;
  cameraToWorld.data_[2][3] = pos.z;
  cameraToWorld.data_[3][3] = 1;
  // <<Initialize first three columns of viewing matrix>>
  v3 dir = v3(look - pos).normalized();
  v3 right = cross(up.normalized(), dir).normalized();
  v3 newUp = cross(dir, right).normalized();
  cameraToWorld.data_[0][0] = right.x;
  cameraToWorld.data_[1][0] = right.y;
  cameraToWorld.data_[2][0] = right.z;
  cameraToWorld.data_[3][0] = 0.;
  cameraToWorld.data_[0][1] = newUp.x;
  cameraToWorld.data_[1][1] = newUp.y;
  cameraToWorld.data_[2][1] = newUp.z;
  cameraToWorld.data_[3][1] = 0.;
  cameraToWorld.data_[0][2] = dir.x;
  cameraToWorld.data_[1][2] = dir.y;
  cameraToWorld.data_[2][2] = dir.z;
  cameraToWorld.data_[3][2] = 0.;
  return Transform(cameraToWorld.inverse(), cameraToWorld);
}

}  // namespace transforms

}  // namespace cs::linalg

#endif  // CS_LINALG_TRANSFORM_HH
