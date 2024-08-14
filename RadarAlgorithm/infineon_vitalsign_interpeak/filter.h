/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: filter.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 28-Feb-2023 17:18:31
 */

#ifndef FILTER_H
#define FILTER_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void filter(const double b[5], const double a[5], const float x[256],
            float y[256]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for filter.h
 *
 * [EOF]
 */
