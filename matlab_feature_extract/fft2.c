/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft2.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
 */

/* Include Files */
#include "fft2.h"
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const float x[2048]
 *                creal32_T y[2048]
 * Return Type  : void
 */
void fft2(const float x[2048], creal32_T y[2048])
{
  static const float fv[17] = {
      1.0F,          0.980785251F,  0.923879504F,  0.831469595F,  0.707106769F,
      0.555570245F,  0.382683456F,  0.195090324F,  0.0F,          -0.195090324F,
      -0.382683456F, -0.555570245F, -0.707106769F, -0.831469595F, -0.923879504F,
      -0.980785251F, -1.0F};
  static const float fv1[17] = {
      0.0F,          -0.195090324F, -0.382683456F, -0.555570245F, -0.707106769F,
      -0.831469595F, -0.923879504F, -0.980785251F, -1.0F,         -0.980785251F,
      -0.923879504F, -0.831469595F, -0.707106769F, -0.555570245F, -0.382683456F,
      -0.195090324F, -0.0F};
static  creal32_T acc[2048];
static  creal32_T xPerm[2048];
  int b_i;
  int chan;
  int i;
  int iy;
  int j;
  for (chan = 0; chan < 32; chan++) {
    c_FFTImplementationCallback_doH(x, chan << 6,
                                    *(creal32_T(*)[64]) & acc[chan << 6]);
  }
  for (i = 0; i < 64; i++) {
    for (iy = 0; iy < 32; iy++) {
      xPerm[iy + (i << 5)] = acc[i + (iy << 6)];
    }
  }
  for (chan = 0; chan < 64; chan++) {
    float im;
    float re;
    float temp_im;
    float temp_re;
    float twid_im;
    float twid_re;
    int iheight;
    int ix_tmp_tmp;
    int ju;
    int k;
    ix_tmp_tmp = chan << 5;
    iy = 0;
    ju = 0;
    for (b_i = 0; b_i < 31; b_i++) {
      bool tst;
      acc[iy + ix_tmp_tmp] = xPerm[ix_tmp_tmp + b_i];
      iy = 32;
      tst = true;
      while (tst) {
        iy >>= 1;
        ju ^= iy;
        tst = ((ju & iy) == 0);
      }
      iy = ju;
    }
    acc[iy + ix_tmp_tmp] = xPerm[ix_tmp_tmp + 31];
    for (b_i = 0; b_i <= 30; b_i += 2) {
      iy = b_i + ix_tmp_tmp;
      re = acc[iy + 1].re;
      im = acc[iy + 1].im;
      temp_re = re;
      temp_im = im;
      twid_re = acc[iy].re;
      twid_im = acc[iy].im;
      re = twid_re - re;
      im = twid_im - im;
      acc[iy + 1].re = re;
      acc[iy + 1].im = im;
      twid_re += temp_re;
      twid_im += temp_im;
      acc[iy].re = twid_re;
      acc[iy].im = twid_im;
    }
    iy = 2;
    ju = 4;
    k = 8;
    iheight = 29;
    while (k > 0) {
      int istart;
      int temp_re_tmp;
      for (b_i = 0; b_i < iheight; b_i += ju) {
        temp_re_tmp = (b_i + iy) + ix_tmp_tmp;
        temp_re = acc[temp_re_tmp].re;
        temp_im = acc[temp_re_tmp].im;
        i = b_i + ix_tmp_tmp;
        acc[temp_re_tmp].re = acc[i].re - temp_re;
        acc[temp_re_tmp].im = acc[i].im - temp_im;
        acc[i].re += temp_re;
        acc[i].im += temp_im;
      }
      istart = 1;
      for (j = k; j < 16; j += k) {
        int ihi;
        twid_re = fv[j];
        twid_im = fv1[j];
        b_i = istart;
        ihi = istart + iheight;
        while (b_i < ihi) {
          temp_re_tmp = (b_i + iy) + ix_tmp_tmp;
          re = acc[temp_re_tmp].im;
          im = acc[temp_re_tmp].re;
          temp_re = twid_re * im - twid_im * re;
          temp_im = twid_re * re + twid_im * im;
          i = b_i + ix_tmp_tmp;
          acc[temp_re_tmp].re = acc[i].re - temp_re;
          acc[temp_re_tmp].im = acc[i].im - temp_im;
          acc[i].re += temp_re;
          acc[i].im += temp_im;
          b_i += ju;
        }
        istart++;
      }
      k /= 2;
      iy = ju;
      ju += ju;
      iheight -= iy;
    }
  }
  for (i = 0; i < 32; i++) {
    for (iy = 0; iy < 64; iy++) {
      y[iy + (i << 6)] = acc[i + (iy << 5)];
    }
  }
}

/*
 * File trailer for fft2.c
 *
 * [EOF]
 */
