/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: filter.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 28-Feb-2023 17:18:31
 */

/* Include Files */
#include "filter.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double b[5]
 *                const double a[5]
 *                const float x[256]
 *                float y[256]
 * Return Type  : void
 */
void filter(const double b[5], const double a[5], const float x[256],
            float y[256])
{
  float as;
  int j;
  int k;
  int naxpy;
  int y_tmp;
  memset(&y[0], 0, 256U * sizeof(float));
  for (k = 0; k < 256; k++) {
    if (256 - k < 5) {
      naxpy = 255 - k;
    } else {
      naxpy = 4;
    }
    for (j = 0; j <= naxpy; j++) {
      y_tmp = k + j;
      y[y_tmp] += x[k] * (float)b[j];
    }
    if (255 - k < 4) {
      naxpy = 254 - k;
    } else {
      naxpy = 3;
    }
    as = -y[k];
    for (j = 0; j <= naxpy; j++) {
      y_tmp = (k + j) + 1;
      y[y_tmp] += as * (float)a[j + 1];
    }
  }
}

/*
 * File trailer for filter.c
 *
 * [EOF]
 */
