/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_start_judge_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 14:10:44
 */

#ifndef _CODER_START_JUDGE_API_H
#define _CODER_START_JUDGE_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
boolean_T start_judge(uint16_T Data[2048]);

void start_judge_api(const mxArray *prhs, const mxArray **plhs);

void start_judge_atexit(void);

void start_judge_initialize(void);

void start_judge_terminate(void);

void start_judge_xil_shutdown(void);

void start_judge_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_start_judge_api.h
 *
 * [EOF]
 */
