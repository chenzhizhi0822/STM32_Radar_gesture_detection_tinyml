/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interpeaks.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 28-Feb-2023 17:18:31
 */

/* Include Files */
#include "interpeaks.h"
#include "FFTImplementationCallback.h"
#include "filter.h"
#include "findpeaks.h"
#include "interpeaks_data.h"
#include "interpeaks_initialize.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static float phaseBuffer[256];

/* Function Declarations */
static float rt_atan2f_snf(float u0, float u1);

static float rt_hypotf_snf(float u0, float u1);

/* Function Definitions */
/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
static float rt_atan2f_snf(float u0, float u1)
{
  float y;
  int b_u0;
  int b_u1;
  if (rtIsNaNF(u0) || rtIsNaNF(u1)) {
    y = rtNaNF;
  } else if (rtIsInfF(u0) && rtIsInfF(u1)) {
    if (u0 > 0.0F) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }
    if (u1 > 0.0F) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }
    y = (float)atan2((float)b_u0, (float)b_u1);
  } else if (u1 == 0.0F) {
    if (u0 > 0.0F) {
      y = RT_PIF / 2.0F;
    } else if (u0 < 0.0F) {
      y = -(RT_PIF / 2.0F);
    } else {
      y = 0.0F;
    }
  } else {
    y = (float)atan2(u0, u1);
  }
  return y;
}

/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
static float rt_hypotf_snf(float u0, float u1)
{
  float a;
  float y;
  a = (float)fabs(u0);
  y = (float)fabs(u1);
  if (a < y) {
    a /= y;
    y *= (float)sqrt(a * a + 1.0F);
  } else if (a > y) {
    y /= a;
    y = a * (float)sqrt(y * y + 1.0F);
  } else if (!rtIsNaNF(y)) {
    y = a * 1.41421354F;
  }
  return y;
}

/*
 * tic;
 *
 * Arguments    : const unsigned short adcDataRow[128]
 *                float *heart_est_inter
 *                float *breath_est_inter
 * Return Type  : void
 */
void interpeaks(const unsigned short adcDataRow[128], float *heart_est_inter,
                float *breath_est_inter)
{
  static const double dv[5] = {0.0019013594933556097, 0.0,
                               -0.0038027189867112193, 0.0,
                               0.0019013594933556097};
  static const double dv1[5] = {1.0, -3.9018233021208659, 5.7262803454085951,
                                -3.7466598739850032, 0.922226212902835};
  static const double dv2[5] = {0.015875914972992084, 0.0,
                                -0.031751829945984168, 0.0,
                                0.015875914972992084};
  static const double dv3[5] = {1.0, -3.4046571522551696, 4.6505685964807721,
                                -3.0100826942817078, 0.78582345738106174};
  creal_T y[128];
  creal32_T mean_sig_ffta[128];
  creal32_T sig_ffta[128];
  double breath_locs_data[256];
  double b_adcDataRow[128];
  double pkDiff_breath;
  float a__2_data[256];
  float fv[256];
  float varargin_1[64];
  float mean_ffta_im;
  float mean_ffta_re;
  float re;
  int i;
  int idx;
  int k;
  boolean_T exitg1;
  if (!isInitialized_interpeaks) {
    interpeaks_initialize();
  }
  /* 加窗 */
  /* ADC采样点数 */
  /*  fft_data_last=zeros(1,Nr); */
  /*  range_max=0; */
  /*  Buffer */
  /*  FFT */
  for (i = 0; i < 128; i++) {
    b_adcDataRow[i] = adcDataRow[i];
  }
  c_FFTImplementationCallback_doH(b_adcDataRow, y);
  for (i = 0; i < 128; i++) {
    sig_ffta[i].re = (float)y[i].re;
    sig_ffta[i].im = (float)y[i].im;
  }
  /*  平均相消 */
  mean_ffta_re = sig_ffta[0].re;
  mean_ffta_im = sig_ffta[0].im;
  for (k = 0; k < 127; k++) {
    mean_ffta_re += sig_ffta[k + 1].re;
    mean_ffta_im += sig_ffta[k + 1].im;
  }
  if (mean_ffta_im == 0.0F) {
    re = mean_ffta_re / 128.0F;
    mean_ffta_re = 0.0F;
  } else if (mean_ffta_re == 0.0F) {
    re = 0.0F;
    mean_ffta_re = mean_ffta_im / 128.0F;
  } else {
    re = mean_ffta_re / 128.0F;
    mean_ffta_re = mean_ffta_im / 128.0F;
  }
  for (i = 0; i < 128; i++) {
    mean_sig_ffta[i].re = sig_ffta[i].re - re;
    mean_sig_ffta[i].im = sig_ffta[i].im - mean_ffta_re;
  }
  mean_sig_ffta[0].re = 0.0F;
  mean_sig_ffta[0].im = 0.0F;
  /*  sig_ffta(1) = 0; */
  /*  取出能量最大点的相位  extract phase from selected range bin */
  /*  angle_fft_last = angle_fft(:,max_num); */
  for (k = 0; k < 64; k++) {
    varargin_1[k] = rt_hypotf_snf(mean_sig_ffta[k].re, mean_sig_ffta[k].im);
  }
  if (!rtIsNaNF(varargin_1[0])) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 65)) {
      if (!rtIsNaNF(varargin_1[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (idx == 0) {
    idx = 1;
  } else {
    mean_ffta_re = varargin_1[idx - 1];
    i = idx + 1;
    for (k = i; k < 65; k++) {
      mean_ffta_im = varargin_1[k - 1];
      if (mean_ffta_re < mean_ffta_im) {
        mean_ffta_re = mean_ffta_im;
        idx = k;
      }
    }
  }
  /*  相位解缠绕 */
  /*  angle_fft_last2=unwrap(angle_fft_last); */
  /*  相位差分 */
  i = 1;
  for (k = 0; k < 255; k++) {
    i = k + 1;
    phaseBuffer[k] = phaseBuffer[k + 1];
  }
  phaseBuffer[i] = rt_atan2f_snf(sig_ffta[idx - 1].im, sig_ffta[idx - 1].re);
  /*  phaseBuffer2 = single(zeros(windowlength,1)); */
  /*  for i = 1:windowlength-1 */
  /*      phaseBuffer2(i) = phaseBuffer(i+1)-phaseBuffer(i); */
  /*      phaseBuffer2(windowlength) =
   * phaseBuffer(windowlength)-phaseBuffer(windowlength-1); */
  /*  end */
  /*  滤波 */
  /*  IIR filter */
  /*  inter-peaks breath */
  filter(dv, dv1, phaseBuffer, fv);
  findpeaks(fv, a__2_data, &i, breath_locs_data, &idx);
  /*  pk_Diff_breath = zeros(length(breath_locs),1);  */
  /*  peakLocsOut_breath = single(zeros(4,1));    */
  i = 1;
  for (k = 0; k <= idx - 2; k++) {
    pkDiff_breath = breath_locs_data[k + 1] - breath_locs_data[k];
    if ((pkDiff_breath > 40.0) && (pkDiff_breath < 200.0)) {
      i++;
    }
  }
  *breath_est_inter = (float)(60.0 * ((double)i - 1.0) * 20.0 / 256.0);
  /*  inter-peaks heart */
  /*  IIR filter */
  filter(dv2, dv3, phaseBuffer, fv);
  findpeaks(fv, a__2_data, &i, breath_locs_data, &idx);
  /*  pk_Diff_heart = zeros(length(heart_locs),1);  */
  /*  peakLocsOut_heart = single(zeros(12,1));  */
  i = 1;
  for (k = 0; k <= idx - 2; k++) {
    pkDiff_breath = breath_locs_data[k + 1] - breath_locs_data[k];
    if ((pkDiff_breath > 10.0) && (pkDiff_breath < 25.0)) {
      i++;
    }
  }
  /*  A = size(peakLocsOut_heart,1); */
  /*  peakLocsOut_heart = length(peakLocsOut_heart);   */
  *heart_est_inter = (float)(60.0 * ((double)i - 1.0) * 20.0 / 256.0);
  /*  toc; */
  /*  disp(['运行时间: ',num2str(toc)]); */
  /*  disp(['呼吸inter-pks:',num2str(breath_est_inter),... */
  /*         '心跳inter-pks:',num2str(heart_est_inter)]); */
}

/*
 * tic;
 *
 * Arguments    : void
 * Return Type  : void
 */
void interpeaks_init(void)
{
  memset(&phaseBuffer[0], 0, 256U * sizeof(float));
}

/*
 * File trailer for interpeaks.c
 *
 * [EOF]
 */
