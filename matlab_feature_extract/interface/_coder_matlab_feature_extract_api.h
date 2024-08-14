/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_matlab_feature_extract_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
 */

#ifndef _CODER_MATLAB_FEATURE_EXTRACT_API_H
#define _CODER_MATLAB_FEATURE_EXTRACT_API_H

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
void matlab_feature_extract(uint16_T Data[6144], uint32_T frame_idx,
                            real32_T featuresMatrix[6]);

void matlab_feature_extract_api(const mxArray *const prhs[2],
                                const mxArray **plhs);

void matlab_feature_extract_atexit(void);

void matlab_feature_extract_initialize(void);

void matlab_feature_extract_terminate(void);

void matlab_feature_extract_xil_shutdown(void);

void matlab_feature_extract_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_matlab_feature_extract_api.h
 *
 * [EOF]
 */
