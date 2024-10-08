/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_matlab_feature_extract_mex.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
 */

#ifndef _CODER_MATLAB_FEATURE_EXTRACT_MEX_H
#define _CODER_MATLAB_FEATURE_EXTRACT_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS(void);

void unsafe_matlab_feature_extract_mexFunction(int32_T nlhs, mxArray *plhs[1],
                                               int32_T nrhs,
                                               const mxArray *prhs[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_matlab_feature_extract_mex.h
 *
 * [EOF]
 */
