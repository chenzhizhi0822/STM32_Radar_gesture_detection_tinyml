/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ind2sub.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
 */

/* Include Files */
#include "ind2sub.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const float ndx[10]
 *                int varargout_1[10]
 *                int varargout_2[10]
 * Return Type  : void
 */
void ind2sub_indexClass(const float ndx[10], int varargout_1[10],
                        int varargout_2[10])
{
  int i;
  for (i = 0; i < 10; i++) {
    int b_i;
    int i1;
    b_i = (int)ndx[i];
    i1 = (b_i - 1) / 32;
    varargout_2[i] = i1 + 1;
    varargout_1[i] = b_i - (i1 << 5);
  }
}

/*
 * File trailer for ind2sub.c
 *
 * [EOF]
 */
