/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interpeaks_initialize.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 28-Feb-2023 17:18:31
 */

/* Include Files */
#include "interpeaks_initialize.h"
#include "interpeaks.h"
#include "interpeaks_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void interpeaks_initialize(void)
{
  rt_InitInfAndNaN();
  interpeaks_init();
  isInitialized_interpeaks = true;
}

/*
 * File trailer for interpeaks_initialize.c
 *
 * [EOF]
 */
