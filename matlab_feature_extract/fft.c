/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
 */

/* Include Files */
#include "fft.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T x[2]
 *                creal32_T y[3]
 * Return Type  : void
 */
void fft(const creal32_T x[2], creal32_T y[3])
{
  static const creal32_T wwc[5] = {{
                                       -0.499999911F, /* re */
                                       -0.866025448F  /* im */
                                   },
                                   {
                                       0.49999997F, /* re */
                                       0.866025448F /* im */
                                   },
                                   {
                                       1.0F, /* re */
                                       0.0F  /* im */
                                   },
                                   {
                                       0.49999997F, /* re */
                                       0.866025448F /* im */
                                   },
                                   {
                                       -0.499999911F, /* re */
                                       -0.866025448F  /* im */
                                   }};
  static const float b_fv[5] = {1.0F, 0.707106769F, 0.0F, -0.707106769F, -1.0F};
  static const float fv1[5] = {0.0F, -0.707106769F, -1.0F, -0.707106769F,
                               -0.0F};
  static const float fv2[5] = {0.0F, 0.707106769F, 1.0F, 0.707106769F, 0.0F};
  creal32_T fv[8];
  creal32_T fy[8];
  float re_tmp;
  float temp_im;
  float temp_re;
  float temp_re_tmp_tmp;
  float twid_im;
  float twid_re;
  int i;
  int iDelta;
  int iheight;
  int ihi;
  int istart;
  int j;
  int ju;
  int k;
  int temp_re_tmp;
  bool tst;
  y[0].re = wwc[2].re * x[0].re + wwc[2].im * x[0].im;
  y[0].im = wwc[2].re * x[0].im - wwc[2].im * x[0].re;
  y[1].re = wwc[3].re * x[1].re + wwc[3].im * x[1].im;
  y[1].im = wwc[3].re * x[1].im - wwc[3].im * x[1].re;
  y[2].re = 0.0F;
  y[2].im = 0.0F;
  memset(&fy[0], 0, 8U * sizeof(creal32_T));
  ju = 0;
  fy[0] = y[0];
  iDelta = 8;
  tst = true;
  while (tst) {
    iDelta >>= 1;
    ju ^= iDelta;
    tst = ((ju & iDelta) == 0);
  }
  fy[ju] = y[1];
  iDelta = 8;
  tst = true;
  while (tst) {
    iDelta >>= 1;
    ju ^= iDelta;
    tst = ((ju & iDelta) == 0);
  }
  fy[ju] = y[2];
  temp_re_tmp_tmp = fy[1].re;
  temp_im = fy[1].im;
  fy[1].re = fy[0].re - fy[1].re;
  fy[1].im = fy[0].im - fy[1].im;
  fy[0].re += temp_re_tmp_tmp;
  fy[0].im += temp_im;
  temp_re_tmp_tmp = fy[3].re;
  temp_im = fy[3].im;
  fy[3].re = fy[2].re - fy[3].re;
  fy[3].im = fy[2].im - fy[3].im;
  fy[2].re += temp_re_tmp_tmp;
  fy[2].im += temp_im;
  temp_re_tmp_tmp = fy[5].re;
  temp_im = fy[5].im;
  fy[5].re = fy[4].re - fy[5].re;
  fy[5].im = fy[4].im - fy[5].im;
  fy[4].re += temp_re_tmp_tmp;
  fy[4].im += temp_im;
  temp_re_tmp_tmp = fy[7].re;
  temp_im = fy[7].im;
  fy[7].re = fy[6].re - fy[7].re;
  fy[7].im = fy[6].im - fy[7].im;
  fy[6].re += temp_re_tmp_tmp;
  fy[6].im += temp_im;
  iDelta = 2;
  ju = 4;
  k = 2;
  iheight = 5;
  while (k > 0) {
    for (i = 0; i < iheight; i += ju) {
      temp_re_tmp = i + iDelta;
      temp_re = fy[temp_re_tmp].re;
      temp_im = fy[temp_re_tmp].im;
      fy[temp_re_tmp].re = fy[i].re - temp_re;
      fy[temp_re_tmp].im = fy[i].im - temp_im;
      fy[i].re += temp_re;
      fy[i].im += temp_im;
    }
    istart = 1;
    for (j = k; j < 4; j += k) {
      twid_re = b_fv[j];
      twid_im = fv1[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iDelta;
        temp_re_tmp_tmp = fy[temp_re_tmp].im;
        re_tmp = fy[temp_re_tmp].re;
        temp_re = twid_re * re_tmp - twid_im * temp_re_tmp_tmp;
        temp_im = twid_re * temp_re_tmp_tmp + twid_im * re_tmp;
        fy[temp_re_tmp].re = fy[i].re - temp_re;
        fy[temp_re_tmp].im = fy[i].im - temp_im;
        fy[i].re += temp_re;
        fy[i].im += temp_im;
        i += ju;
      }
      istart++;
    }
    k /= 2;
    iDelta = ju;
    ju += ju;
    iheight -= iDelta;
  }
  memset(&fv[0], 0, 8U * sizeof(creal32_T));
  ju = 0;
  fv[0] = wwc[0];
  iDelta = 8;
  tst = true;
  while (tst) {
    iDelta >>= 1;
    ju ^= iDelta;
    tst = ((ju & iDelta) == 0);
  }
  fv[ju] = wwc[1];
  iDelta = 8;
  tst = true;
  while (tst) {
    iDelta >>= 1;
    ju ^= iDelta;
    tst = ((ju & iDelta) == 0);
  }
  fv[ju] = wwc[2];
  iDelta = 8;
  tst = true;
  while (tst) {
    iDelta >>= 1;
    ju ^= iDelta;
    tst = ((ju & iDelta) == 0);
  }
  fv[ju] = wwc[3];
  iDelta = 8;
  tst = true;
  while (tst) {
    iDelta >>= 1;
    ju ^= iDelta;
    tst = ((ju & iDelta) == 0);
  }
  fv[ju] = wwc[4];
  temp_re_tmp_tmp = fv[1].re;
  temp_im = fv[1].im;
  fv[1].re = fv[0].re - fv[1].re;
  fv[1].im = fv[0].im - fv[1].im;
  fv[0].re += temp_re_tmp_tmp;
  fv[0].im += temp_im;
  temp_re_tmp_tmp = fv[3].re;
  temp_im = fv[3].im;
  fv[3].re = fv[2].re - fv[3].re;
  fv[3].im = fv[2].im - fv[3].im;
  fv[2].re += temp_re_tmp_tmp;
  fv[2].im += temp_im;
  temp_re_tmp_tmp = fv[5].re;
  temp_im = fv[5].im;
  fv[5].re = fv[4].re - fv[5].re;
  fv[5].im = fv[4].im - fv[5].im;
  fv[4].re += temp_re_tmp_tmp;
  fv[4].im += temp_im;
  temp_re_tmp_tmp = fv[7].re;
  temp_im = fv[7].im;
  fv[7].re = fv[6].re - fv[7].re;
  fv[7].im = fv[6].im - fv[7].im;
  fv[6].re += temp_re_tmp_tmp;
  fv[6].im += temp_im;
  iDelta = 2;
  ju = 4;
  k = 2;
  iheight = 5;
  while (k > 0) {
    for (i = 0; i < iheight; i += ju) {
      temp_re_tmp = i + iDelta;
      temp_re = fv[temp_re_tmp].re;
      temp_im = fv[temp_re_tmp].im;
      fv[temp_re_tmp].re = fv[i].re - temp_re;
      fv[temp_re_tmp].im = fv[i].im - temp_im;
      fv[i].re += temp_re;
      fv[i].im += temp_im;
    }
    istart = 1;
    for (j = k; j < 4; j += k) {
      twid_re = b_fv[j];
      twid_im = fv1[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iDelta;
        temp_re_tmp_tmp = fv[temp_re_tmp].im;
        re_tmp = fv[temp_re_tmp].re;
        temp_re = twid_re * re_tmp - twid_im * temp_re_tmp_tmp;
        temp_im = twid_re * temp_re_tmp_tmp + twid_im * re_tmp;
        fv[temp_re_tmp].re = fv[i].re - temp_re;
        fv[temp_re_tmp].im = fv[i].im - temp_im;
        fv[i].re += temp_re;
        fv[i].im += temp_im;
        i += ju;
      }
      istart++;
    }
    k /= 2;
    iDelta = ju;
    ju += ju;
    iheight -= iDelta;
  }
  for (iDelta = 0; iDelta < 8; iDelta++) {
    temp_re_tmp_tmp = fy[iDelta].re;
    temp_im = fv[iDelta].im;
    re_tmp = fy[iDelta].im;
    temp_re = fv[iDelta].re;
    fy[iDelta].re = temp_re_tmp_tmp * temp_re - re_tmp * temp_im;
    fy[iDelta].im = temp_re_tmp_tmp * temp_im + re_tmp * temp_re;
  }
  iDelta = 0;
  ju = 0;
  for (i = 0; i < 7; i++) {
    fv[iDelta] = fy[i];
    iDelta = 8;
    tst = true;
    while (tst) {
      iDelta >>= 1;
      ju ^= iDelta;
      tst = ((ju & iDelta) == 0);
    }
    iDelta = ju;
  }
  fv[iDelta] = fy[7];
  temp_re_tmp_tmp = fv[1].re;
  temp_im = fv[1].im;
  fv[1].re = fv[0].re - fv[1].re;
  fv[1].im = fv[0].im - fv[1].im;
  fv[0].re += temp_re_tmp_tmp;
  fv[0].im += temp_im;
  temp_re_tmp_tmp = fv[3].re;
  temp_im = fv[3].im;
  fv[3].re = fv[2].re - fv[3].re;
  fv[3].im = fv[2].im - fv[3].im;
  fv[2].re += temp_re_tmp_tmp;
  fv[2].im += temp_im;
  temp_re_tmp_tmp = fv[5].re;
  temp_im = fv[5].im;
  fv[5].re = fv[4].re - fv[5].re;
  fv[5].im = fv[4].im - fv[5].im;
  fv[4].re += temp_re_tmp_tmp;
  fv[4].im += temp_im;
  temp_re_tmp_tmp = fv[7].re;
  temp_im = fv[7].im;
  fv[7].re = fv[6].re - fv[7].re;
  fv[7].im = fv[6].im - fv[7].im;
  fv[6].re += temp_re_tmp_tmp;
  fv[6].im += temp_im;
  iDelta = 2;
  ju = 4;
  k = 2;
  iheight = 5;
  while (k > 0) {
    for (i = 0; i < iheight; i += ju) {
      temp_re_tmp = i + iDelta;
      temp_re = fv[temp_re_tmp].re;
      temp_im = fv[temp_re_tmp].im;
      fv[temp_re_tmp].re = fv[i].re - temp_re;
      fv[temp_re_tmp].im = fv[i].im - temp_im;
      fv[i].re += temp_re;
      fv[i].im += temp_im;
    }
    istart = 1;
    for (j = k; j < 4; j += k) {
      twid_re = b_fv[j];
      twid_im = fv2[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re_tmp = i + iDelta;
        temp_re_tmp_tmp = fv[temp_re_tmp].im;
        re_tmp = fv[temp_re_tmp].re;
        temp_re = twid_re * re_tmp - twid_im * temp_re_tmp_tmp;
        temp_im = twid_re * temp_re_tmp_tmp + twid_im * re_tmp;
        fv[temp_re_tmp].re = fv[i].re - temp_re;
        fv[temp_re_tmp].im = fv[i].im - temp_im;
        fv[i].re += temp_re;
        fv[i].im += temp_im;
        i += ju;
      }
      istart++;
    }
    k /= 2;
    iDelta = ju;
    ju += ju;
    iheight -= iDelta;
  }
  for (iDelta = 0; iDelta < 8; iDelta++) {
    fv[iDelta].re *= 0.125F;
    fv[iDelta].im *= 0.125F;
  }
  y[0].re = wwc[2].re * fv[2].re + wwc[2].im * fv[2].im;
  y[0].im = wwc[2].re * fv[2].im - wwc[2].im * fv[2].re;
  y[1].re = wwc[3].re * fv[3].re + wwc[3].im * fv[3].im;
  y[1].im = wwc[3].re * fv[3].im - wwc[3].im * fv[3].re;
  y[2].re = wwc[4].re * fv[4].re + wwc[4].im * fv[4].im;
  y[2].im = wwc[4].re * fv[4].im - wwc[4].im * fv[4].re;
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */
