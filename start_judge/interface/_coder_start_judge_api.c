/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_start_judge_api.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 14:10:44
 */

/* Include Files */
#include "_coder_start_judge_api.h"
#include "_coder_start_judge_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131627U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "start_judge",                                        /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

/* Function Declarations */
static uint16_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                     const emlrtMsgIdentifier *parentId))[2048];

static uint16_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                     const emlrtMsgIdentifier *msgId))[2048];

static uint16_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *Data,
                                   const char_T *identifier))[2048];

static const mxArray *emlrt_marshallOut(const boolean_T u);

/* Function Definitions */
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : uint16_T (*)[2048]
 */
static uint16_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                     const emlrtMsgIdentifier *parentId))[2048]
{
  uint16_T(*y)[2048];
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : uint16_T (*)[2048]
 */
static uint16_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                     const emlrtMsgIdentifier *msgId))[2048]
{
  static const int32_T dims[2] = {64, 32};
  uint16_T(*ret)[2048];
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "uint16", false, 2U,
                          (const void *)&dims[0]);
  ret = (uint16_T(*)[2048])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *Data
 *                const char_T *identifier
 * Return Type  : uint16_T (*)[2048]
 */
static uint16_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *Data,
                                   const char_T *identifier))[2048]
{
  emlrtMsgIdentifier thisId;
  uint16_T(*y)[2048];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(Data), &thisId);
  emlrtDestroyArray(&Data);
  return y;
}

/*
 * Arguments    : const boolean_T u
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const boolean_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateLogicalScalar(u);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : const mxArray *prhs
 *                const mxArray **plhs
 * Return Type  : void
 */
void start_judge_api(const mxArray *prhs, const mxArray **plhs)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  uint16_T(*Data)[2048];
  boolean_T flag;
  st.tls = emlrtRootTLSGlobal;
  /* Marshall function inputs */
  Data = emlrt_marshallIn(&st, emlrtAlias(prhs), "Data");
  /* Invoke the target function */
  flag = start_judge(*Data);
  /* Marshall function outputs */
  *plhs = emlrt_marshallOut(flag);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void start_judge_atexit(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  start_judge_xil_terminate();
  start_judge_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void start_judge_initialize(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, NULL);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void start_judge_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_start_judge_api.c
 *
 * [EOF]
 */
