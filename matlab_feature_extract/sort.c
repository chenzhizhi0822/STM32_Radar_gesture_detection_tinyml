/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sort.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
 */

/* Include Files */
#include "sort.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : short x[1024]
 *                int idx[1024]
 * Return Type  : void
 */
void sort(short x[1024], int idx[1024])
{
  int iwork[1024];
  int b;
  int bLen;
  int bLen2;
  int b_b;
  int b_i2;
  int b_i3;
  int i;
  int i1;
  int i2;
  int i4;
  int j;
  int k;
  short xwork[1024];
  short idx4[4];
  short x4[4];
  for (j = 0; j < 256; j++) {
    short b_i;
    short b_i1;
    short b_i4;
    short i3;
    signed char i5;
    signed char i6;
    signed char i7;
    signed char i8;
    i = j << 2;
    idx4[0] = (short)(i + 1);
    idx4[1] = (short)(i + 2);
    idx4[2] = (short)(i + 3);
    idx4[3] = (short)(i + 4);
    b_i = x[i];
    x4[0] = b_i;
    b_i1 = x[i + 1];
    x4[1] = b_i1;
    i3 = x[i + 2];
    x4[2] = i3;
    b_i4 = x[i + 3];
    x4[3] = b_i4;
    if (b_i >= b_i1) {
      i1 = 1;
      b_i2 = 2;
    } else {
      i1 = 2;
      b_i2 = 1;
    }
    if (i3 >= b_i4) {
      b_i3 = 3;
      i4 = 4;
    } else {
      b_i3 = 4;
      i4 = 3;
    }
    i2 = x4[i1 - 1];
    b = x4[b_i3 - 1];
    if (i2 >= b) {
      i2 = x4[b_i2 - 1];
      if (i2 >= b) {
        i5 = (signed char)i1;
        i6 = (signed char)b_i2;
        i7 = (signed char)b_i3;
        i8 = (signed char)i4;
      } else if (i2 >= x4[i4 - 1]) {
        i5 = (signed char)i1;
        i6 = (signed char)b_i3;
        i7 = (signed char)b_i2;
        i8 = (signed char)i4;
      } else {
        i5 = (signed char)i1;
        i6 = (signed char)b_i3;
        i7 = (signed char)i4;
        i8 = (signed char)b_i2;
      }
    } else {
      b = x4[i4 - 1];
      if (i2 >= b) {
        if (x4[b_i2 - 1] >= b) {
          i5 = (signed char)b_i3;
          i6 = (signed char)i1;
          i7 = (signed char)b_i2;
          i8 = (signed char)i4;
        } else {
          i5 = (signed char)b_i3;
          i6 = (signed char)i1;
          i7 = (signed char)i4;
          i8 = (signed char)b_i2;
        }
      } else {
        i5 = (signed char)b_i3;
        i6 = (signed char)i4;
        i7 = (signed char)i1;
        i8 = (signed char)b_i2;
      }
    }
    idx[i] = idx4[i5 - 1];
    idx[i + 1] = idx4[i6 - 1];
    idx[i + 2] = idx4[i7 - 1];
    idx[i + 3] = idx4[i8 - 1];
    x[i] = x4[i5 - 1];
    x[i + 1] = x4[i6 - 1];
    x[i + 2] = x4[i7 - 1];
    x[i + 3] = x4[i8 - 1];
  }
  for (b = 0; b < 4; b++) {
    short b_iwork[256];
    short b_xwork[256];
    i4 = (b << 8) - 1;
    for (b_b = 0; b_b < 6; b_b++) {
      bLen = 1 << (b_b + 2);
      bLen2 = bLen << 1;
      i2 = 256 >> (b_b + 3);
      for (k = 0; k < i2; k++) {
        b_i2 = (i4 + k * bLen2) + 1;
        for (j = 0; j < bLen2; j++) {
          i = b_i2 + j;
          b_iwork[j] = (short)idx[i];
          b_xwork[j] = x[i];
        }
        b_i3 = 0;
        i1 = bLen;
        i = b_i2 - 1;
        int exitg1;
        do {
          exitg1 = 0;
          i++;
          if (b_xwork[b_i3] >= b_xwork[i1]) {
            idx[i] = b_iwork[b_i3];
            x[i] = b_xwork[b_i3];
            if (b_i3 + 1 < bLen) {
              b_i3++;
            } else {
              exitg1 = 1;
            }
          } else {
            idx[i] = b_iwork[i1];
            x[i] = b_xwork[i1];
            if (i1 + 1 < bLen2) {
              i1++;
            } else {
              i -= b_i3;
              for (j = b_i3 + 1; j <= bLen; j++) {
                i1 = i + j;
                idx[i1] = b_iwork[j - 1];
                x[i1] = b_xwork[j - 1];
              }
              exitg1 = 1;
            }
          }
        } while (exitg1 == 0);
      }
    }
  }
  memset(&iwork[0], 0, 1024U * sizeof(int));
  memset(&xwork[0], 0, 1024U * sizeof(short));
  i1 = 4;
  bLen = 256;
  while (i1 > 1) {
    if ((i1 & 1) != 0) {
      i1--;
      i = bLen * i1;
      if (1024 - i > bLen) {
        merge(idx, x, i, bLen, 1024 - (i + bLen), iwork, xwork);
      }
    }
    bLen2 = bLen << 1;
    i1 >>= 1;
    for (k = 0; k < i1; k++) {
      merge(idx, x, k * bLen2, bLen, bLen, iwork, xwork);
    }
    bLen = bLen2;
  }
  if (bLen < 1024) {
    merge(idx, x, 0, bLen, 1024 - bLen, iwork, xwork);
  }
}

/*
 * File trailer for sort.c
 *
 * [EOF]
 */
