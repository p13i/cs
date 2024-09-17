// Much of this file is based on:
// https://pbr-book.org/3ed-2018/Geometry_and_Transformations/Transformations

#include "cs/linalg/transform.hh"

#include "cs/geo/cross.hh"
#include "cs/geo/vector3.h"
#include "cs/linalg/matrix4x4.hh"

using m4x4 = ::cs::linalg::Matrix4x4;
using v3 = ::cs::geo::Vector3;
using ::cs::geo::cross;

namespace cs::linalg::transforms {

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

Transform Rotate(float theta_rad, const v3 &axis) {
  v3 a = axis.normalized();
  float sinTheta = std::sin(theta_rad);
  float cosTheta = std::cos(theta_rad);
  Matrix4x4 m;
  // Compute rotation of first basis vector
  m.data_[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
  m.data_[0][1] =
      a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
  m.data_[0][2] =
      a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
  m.data_[0][3] = 0;

  // Compute rotations of second and third basis vectors
  m.data_[1][0] =
      a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
  m.data_[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
  m.data_[1][2] =
      a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
  m.data_[1][3] = 0;

  m.data_[2][0] =
      a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
  m.data_[2][1] =
      a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
  m.data_[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
  m.data_[2][3] = 0;
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
  v3 right =
      cs::geo::cross(up.normalized(), dir).normalized();
  v3 newUp = cs::geo::cross(dir, right).normalized();
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
  return Transform(cameraToWorld.inverse().value(),
                   cameraToWorld);
}

}  // namespace cs::linalg::transforms
