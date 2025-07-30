//===---------------------------------------------------------------------===//
/**
 * @file Mandelbrot.cpp
 * @brief A simple Mandelbrot set generator.
 * @version 0.1
 * @date 2025-07-30
 *
 * @copyright Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <array>
#include <complex>
#include <cstdio>

//===---------------------------------------------------------------------===//
/* Function Definitions */

struct Dimensions {
  int width;
  int height;
};

struct IterationLimit {
  int value;
};
void mandelbrot(std::array<int, 500UL * 500UL>& image, Dimensions dim, IterationLimit max_iter) {
  for (int y = 0; y < dim.height; ++y) {
    for (int x = 0; x < dim.width; ++x) { // <<<<< Breakpoint here
      std::complex<float> xy(
          -2.05f + static_cast<float>(x) * 3.0f / static_cast<float>(dim.width),
          -1.5f + static_cast<float>(y) * 3.0f / static_cast<float>(dim.height));
      std::complex<float> z(0, 0);
      int                 count = max_iter.value;
      for (int i = 0; i < max_iter.value; ++i) {
        z = z * z + xy;
        if (std::abs(z) >= 2) {
          count = i;
          break;
        }
      }
      image[y * dim.width + x] = count;
    }
  }
}
//===---------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  const int                      xdim     = 500;
  const int                      ydim     = 500;
  const int                      max_iter = 100;
  std::array<int, 500UL * 500UL> image    = {0};
  mandelbrot(image, {.width = xdim, .height = ydim}, {max_iter});
  for (int y = 0; y < ydim; y += 10) {
    for (int x = 0; x < xdim; x += 5) {
      putchar(image[y * xdim + x] < max_iter ? '.' : '#');
    }
    putchar('\n');
  }
  return 0;
}
//===---------------------------------------------------------------------===//
