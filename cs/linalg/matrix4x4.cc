#include "cs/linalg/matrix4x4.hh"

#include "cs/numbers/in_range.hh"
#include "cs/sanity/ensure.hh"
#include "cs/sanity/error.hh"

using ::cs::linalg::Matrix4x4;
using ::cs::numbers::in_range;

#include <memory.h>
#include <stdint.h>

#include "matrix4x4.hh"

namespace {

void EnsureIndexArgs(uint8_t x, uint8_t y) {
  ENSURE(in_range<uint8_t>(x, 0, 4));
  ENSURE(in_range<uint8_t>(y, 0, 4));
}

}  // namespace

float cs::linalg::Matrix4x4::get(uint8_t x,
                                 uint8_t y) const {
  EnsureIndexArgs(x, y);
  return data_[x][y];
}

void cs::linalg::Matrix4x4::set(uint8_t x, uint8_t y,
                                float datum) {
  EnsureIndexArgs(x, y);
  data_[x][y] = datum;
}

// Pulled from PBRT:
// https://github.com/mmp/pbrt-v3/blob/aaa552a4b9cbf9dccb71450f47b268e0ed6370e2/src/core/transform.cpp#L82-L136
Matrix4x4 cs::linalg::Matrix4x4::inverse() const {
  int indxc[4], indxr[4];
  int ipiv[4] = {0, 0, 0, 0};
  float minv[4][4];
  memcpy(minv, data_, 4 * 4 * sizeof(float));
  for (int i = 0; i < 4; i++) {
    int irow = 0, icol = 0;
    float big = 0.f;
    // Choose pivot
    for (int j = 0; j < 4; j++) {
      if (ipiv[j] != 1) {
        for (int k = 0; k < 4; k++) {
          if (ipiv[k] == 0) {
            if (std::abs(minv[j][k]) >= big) {
              big = float(std::abs(minv[j][k]));
              irow = j;
              icol = k;
            }
          } else if (ipiv[k] > 1) {
            ERROR("Singular matrix in MatrixInvert");
          }
        }
      }
    }
    ++ipiv[icol];
    // Swap rows _irow_ and _icol_ for pivot
    if (irow != icol) {
      for (int k = 0; k < 4; ++k)
        std::swap(minv[irow][k], minv[icol][k]);
    }
    indxr[i] = irow;
    indxc[i] = icol;
    if (minv[icol][icol] == 0.f) {
      ERROR("Singular matrix in MatrixInvert");
    }

    // Set $m[icol][icol]$ to one by scaling row _icol_
    // appropriately
    float pivinv = 1. / minv[icol][icol];
    minv[icol][icol] = 1.;
    for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

    // Subtract this row from others to zero out their
    // columns
    for (int j = 0; j < 4; j++) {
      if (j != icol) {
        float save = minv[j][icol];
        minv[j][icol] = 0;
        for (int k = 0; k < 4; k++)
          minv[j][k] -= minv[icol][k] * save;
      }
    }
  }
  // Swap columns to reflect permutation
  for (int j = 3; j >= 0; j--) {
    if (indxr[j] != indxc[j]) {
      for (int k = 0; k < 4; k++)
        std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
    }
  }
  return Matrix4x4(minv);
}
