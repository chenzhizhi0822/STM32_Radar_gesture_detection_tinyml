/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interpeaks.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 28-Feb-2023 17:18:31
 */

#ifndef INTERPEAKS_H
#define INTERPEAKS_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void interpeaks(const unsigned short adcDataRow[128],
                       float *heart_est_inter, float *breath_est_inter);

void interpeaks_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for interpeaks.h
 *
 * [EOF]
 */
