/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: matlab_feature_extract.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
 */

/* Include Files */
#include "matlab_feature_extract.h"
#include "fft.h"
#include "fft2.h"
#include "ind2sub.h"
#include "matlab_feature_extract_emxutil.h"
#include "matlab_feature_extract_types.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sum.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static float rt_hypotf_snf(float u0, float u1);

static double rt_roundd_snf(double u);

/* Function Definitions */
/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
static float rt_hypotf_snf(float u0, float u1)
{
  float a;
  float b;
  float y;
  a = fabsf(u0);
  b = fabsf(u1);
  if (a < b) {
    a /= b;
    y = b * sqrtf(a * a + 1.0F);
  } else if (a > b) {
    b /= a;
    y = a * sqrtf(b * b + 1.0F);
  } else if (rtIsNaNF(b)) {
    y = rtNaNF;
  } else {
    y = a * 1.41421354F;
  }
  return y;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }
  return y;
}

/*
 * %%
 *
 * Arguments    : const unsigned short Data[6144]
 *                unsigned int frame_idx
 *                float featuresMatrix[6]
 * Return Type  : void
 */
void matlab_feature_extract(const unsigned short Data[6144],
                            unsigned int frame_idx, float featuresMatrix[6])
{
  static const float a[32] = {
      -416.666656F, -390.624969F, -364.583313F, -338.541656F, -312.5F,
      -286.458313F, -260.416656F, -234.375F,    -208.333328F, -182.291656F,
      -156.25F,     -130.208328F, -104.166664F, -78.125F,     -52.0833321F,
      -26.041666F,  0.0F,         26.041666F,   52.0833321F,  78.125F,
      104.166664F,  130.208328F,  156.25F,      182.291656F,  208.333328F,
      234.375F,     260.416656F,  286.458313F,  312.5F,       338.541656F,
      364.583313F,  390.624969F};
  emxArray_real32_T *azimuth_angles;
  emxArray_real32_T *elevation_angles;
  emxArray_real32_T *weights;
static  creal32_T Data1_fft[2048];
static  creal32_T Data2_fft[2048];
static  creal32_T Data3_fft[2048];
  double y[64];
  double d;
static  float b_data1_short[2048];
static  float c_Data[1024];
static  float noncoherent_values[1024];
  float Zi_tmp[32];
  float d_fcnOutput[32];
  float doppler_bins[10];
  float range_bins[10];
  float V;
  float b_fcnOutput;
  float ex;
  float threshold;
  float tmp_im;
  float tmp_re;
  float weighted_average_azimuth;
  float y_tmp;
  float *azimuth_angles_data;
  float *elevation_angles_data;
  float *weights_data;
  int iidx[1024];
  int varargout_3[10];
  int varargout_4[10];
  int i;
  int i2;
  int j;
  int k;
  int nz;
  unsigned int qY;
  int xj;
  int xoffset;
static  short b_Data[6144];
static  short data1_short[2048];
static  short data2_short[2048];
static  short data3_short[2048];
static  short fcnOutput[2048];
static  short b_x[1024];
  short Data1_mean[64];
  short Data2_mean[64];
  short Data3_mean[64];
  short maxval[32];
  short b_maxval;
  short i1;
  unsigned short q;
  unsigned short x;
  bool c_x[1024];
  qY = frame_idx + 1U;
  if (frame_idx + 1U < frame_idx) {
    qY = MAX_uint32_T;
  }
  /* 64*32*3 */
  for (i = 0; i < 6144; i++) {
    x = Data[i];
    if (x > 32767) {
      x = 32767U;
    }
    b_Data[i] = (short)x;
  }
  /*  */
  /*      n_frames = 1;%帧数 */
  /*  Center frequency */
  /*  For CW, it is 0. */
  /*     %% 英飞凌手势采集数据参数 */
  /*  Number of time samples per sweep采样点数 */
  /* chirp数 */
  /*  record_length=length(Data)/NTS*Tsweep; % length of recording in s */
  /*  扫描时间 */
  /*  光速 */
  /* 波长 */
  /*  天线数量 */
  /*      buffer1 = squeeze(meanBuffer(:,1,:)); */
  /*      buffer2 = squeeze(meanBuffer(:,2,:)); */
  /*      buffer3 = squeeze(meanBuffer(:,3,:)); */
  /*      i = frame_idx; */
  /* meanBuffer = zeros(64,3,15); */
  /*     %% 预定义存储特征值的矩阵 */
  /*      Data1 = zeros(64,32); */
  /*      Data2 = zeros(64,32); */
  /*      Data3 = zeros(64,32); */
  /*       */
  for (xj = 0; xj < 64; xj++) {
    y[xj] = b_Data[xj];
  }
  for (k = 0; k < 31; k++) {
    xoffset = (k + 1) << 6;
    for (xj = 0; xj < 64; xj++) {
      y[xj] += (double)b_Data[xoffset + xj];
    }
  }
  for (xj = 0; xj < 64; xj++) {
    d = rt_roundd_snf(y[xj] / 32.0);
    if (d < 32768.0) {
      i1 = (short)d;
    } else {
      i1 = MAX_int16_T;
    }
    Data1_mean[xj] = i1;
    y[xj] = b_Data[xj + 2048];
  }
  for (k = 0; k < 31; k++) {
    xoffset = (k + 1) << 6;
    for (xj = 0; xj < 64; xj++) {
      y[xj] += (double)b_Data[(xoffset + xj) + 2048];
    }
  }
  for (xj = 0; xj < 64; xj++) {
    d = rt_roundd_snf(y[xj] / 32.0);
    if (d < 32768.0) {
      i1 = (short)d;
    } else {
      i1 = MAX_int16_T;
    }
    Data2_mean[xj] = i1;
    y[xj] = b_Data[xj + 4096];
  }
  for (k = 0; k < 31; k++) {
    xoffset = (k + 1) << 6;
    for (xj = 0; xj < 64; xj++) {
      y[xj] += (double)b_Data[(xoffset + xj) + 4096];
    }
  }
  for (xoffset = 0; xoffset < 64; xoffset++) {
    d = rt_roundd_snf(y[xoffset] / 32.0);
    if (d < 32768.0) {
      i1 = (short)d;
    } else {
      i1 = MAX_int16_T;
    }
    Data3_mean[xoffset] = i1;
  }
  /*   */
  /*      %将均值存入3 buffer，buffer为64x15 */
  /*      if frame_idx>15 */
  /*          buffer1(:,1:end-1) = buffer1(:,2:end); */
  /*          buffer2(:,1:end-1) = buffer2(:,2:end); */
  /*          buffer3(:,1:end-1) = buffer3(:,2:end); */
  /*   */
  /*          buffer1(:,end) = Data1_mean; */
  /*          buffer2(:,end) = Data2_mean; */
  /*          buffer3(:,end) = Data3_mean; */
  /*      else */
  /*          buffer1(:,frame_idx) = Data1_mean; */
  /*          buffer2(:,frame_idx) = Data2_mean; */
  /*          buffer3(:,frame_idx) = Data3_mean;  */
  /*      end */
  /*  计算滑动窗口15帧均值 */
  /*      windowMean1 = int16(mean(buffer1, 2)); */
  /*      windowMean2 = int16(mean(buffer2, 2)); */
  /*      windowMean3 = int16(mean(buffer3, 2)); */
  /* 短窗MTI 每个chirp减去均值 */
  /* 短窗MTI,每个chirp减去均值 */
  /* 短窗MTI */
  for (i = 0; i < 32; i++) {
    for (i2 = 0; i2 < 64; i2++) {
      xoffset = i2 + (i << 6);
      data1_short[xoffset] = (short)(b_Data[xoffset] - Data1_mean[i2]);
      data2_short[xoffset] = (short)(b_Data[xoffset + 2048] - Data2_mean[i2]);
      data3_short[xoffset] = (short)(b_Data[xoffset + 4096] - Data3_mean[i2]);
    }
  }
  /* 短窗MTI */
  for (k = 0; k < 2048; k++) {
    i1 = data1_short[k];
    if (i1 < 0) {
      fcnOutput[k] = (short)-i1;
    } else {
      fcnOutput[k] = i1;
    }
  }
  for (j = 0; j < 32; j++) {
    i = j << 6;
    i1 = fcnOutput[i];
    for (xoffset = 0; xoffset < 63; xoffset++) {
      b_maxval = fcnOutput[(xoffset + i) + 1];
      if (i1 < b_maxval) {
        i1 = b_maxval;
      }
    }
    maxval[j] = i1;
  }
  b_maxval = maxval[0];
  for (k = 0; k < 31; k++) {
    i1 = maxval[k + 1];
    if (b_maxval < i1) {
      b_maxval = i1;
    }
  }
  /* 归一化,防止数值较大的特征主导模型的训练过程，提高模型的稳定性和性能。 */
  for (k = 0; k < 2048; k++) {
    i1 = data1_short[k];
    if (b_maxval == 0) {
      if (i1 == 0) {
        i1 = 0;
      } else if (i1 < 0) {
        i1 = MIN_int16_T;
      } else {
        i1 = MAX_int16_T;
      }
      data1_short[k] = i1;
    } else if (b_maxval != 1) {
      if (i1 >= 0) {
        x = (unsigned short)i1;
      } else {
        x = (unsigned short)-i1;
      }
      if ((unsigned short)b_maxval == 0U) {
        q = MAX_uint16_T;
      } else {
        q = (unsigned short)((unsigned int)x / (unsigned short)b_maxval);
      }
      x = (unsigned short)((unsigned int)x -
                           (unsigned short)((unsigned int)q *
                                            (unsigned short)b_maxval));
      if ((x > 0) && (x >= (int)((unsigned int)(unsigned short)b_maxval >> 1) +
                               ((unsigned short)b_maxval & 1))) {
        q++;
      }
      data1_short[k] = (short)q;
      if (i1 < 0) {
        data1_short[k] = (short)-(short)q;
      }
    }
    i1 = data2_short[k];
    if (i1 < 0) {
      fcnOutput[k] = (short)-i1;
    } else {
      fcnOutput[k] = i1;
    }
  }
  for (j = 0; j < 32; j++) {
    i = j << 6;
    i1 = fcnOutput[i];
    for (xoffset = 0; xoffset < 63; xoffset++) {
      b_maxval = fcnOutput[(xoffset + i) + 1];
      if (i1 < b_maxval) {
        i1 = b_maxval;
      }
    }
    maxval[j] = i1;
  }
  b_maxval = maxval[0];
  for (k = 0; k < 31; k++) {
    i1 = maxval[k + 1];
    if (b_maxval < i1) {
      b_maxval = i1;
    }
  }
  /* 归一化 */
  for (k = 0; k < 2048; k++) {
    i1 = data2_short[k];
    if (b_maxval == 0) {
      if (i1 == 0) {
        i1 = 0;
      } else if (i1 < 0) {
        i1 = MIN_int16_T;
      } else {
        i1 = MAX_int16_T;
      }
      data2_short[k] = i1;
    } else if (b_maxval != 1) {
      if (i1 >= 0) {
        x = (unsigned short)i1;
      } else {
        x = (unsigned short)-i1;
      }
      if ((unsigned short)b_maxval == 0U) {
        q = MAX_uint16_T;
      } else {
        q = (unsigned short)((unsigned int)x / (unsigned short)b_maxval);
      }
      x = (unsigned short)((unsigned int)x -
                           (unsigned short)((unsigned int)q *
                                            (unsigned short)b_maxval));
      if ((x > 0) && (x >= (int)((unsigned int)(unsigned short)b_maxval >> 1) +
                               ((unsigned short)b_maxval & 1))) {
        q++;
      }
      data2_short[k] = (short)q;
      if (i1 < 0) {
        data2_short[k] = (short)-(short)q;
      }
    }
    i1 = data3_short[k];
    if (i1 < 0) {
      fcnOutput[k] = (short)-i1;
    } else {
      fcnOutput[k] = i1;
    }
  }
  for (j = 0; j < 32; j++) {
    i = j << 6;
    i1 = fcnOutput[i];
    for (xoffset = 0; xoffset < 63; xoffset++) {
      b_maxval = fcnOutput[(xoffset + i) + 1];
      if (i1 < b_maxval) {
        i1 = b_maxval;
      }
    }
    maxval[j] = i1;
  }
  b_maxval = maxval[0];
  for (k = 0; k < 31; k++) {
    i1 = maxval[k + 1];
    if (b_maxval < i1) {
      b_maxval = i1;
    }
  }
  /* 归一化 */
  /*      %长窗MTI 减去15帧窗口的平均值 */
  /*      data1_long = Data1 - windowMean1;% */
  /*      data2_long = Data2 - windowMean2; */
  /*      data3_long = Data3 - windowMean3; */
  /*       */
  /*      data1_long =data1_long/ max(max(abs(data1_long))); %%归一化 */
  /*      data2_long =data2_long/ max(max(abs(data2_long))); */
  /*      data3_long =data3_long/ max(max(abs(data3_long))); */
  /*       */
  /*      Data1 = data1_short+data1_long;%加和长短窗值 */
  /*      Data2 = data2_short+data2_long; */
  /*      Data3 = data3_short+data3_long; */
  /*   */
  /*      */
  /*       */
  /*     %% 2DFFT */
  for (k = 0; k < 2048; k++) {
    i1 = data3_short[k];
    if (b_maxval == 0) {
      if (i1 == 0) {
        i1 = 0;
      } else if (i1 < 0) {
        i1 = MIN_int16_T;
      } else {
        i1 = MAX_int16_T;
      }
      data3_short[k] = i1;
    } else if (b_maxval != 1) {
      if (i1 >= 0) {
        x = (unsigned short)i1;
      } else {
        x = (unsigned short)-i1;
      }
      if ((unsigned short)b_maxval == 0U) {
        q = MAX_uint16_T;
      } else {
        q = (unsigned short)((unsigned int)x / (unsigned short)b_maxval);
      }
      x = (unsigned short)((unsigned int)x -
                           (unsigned short)((unsigned int)q *
                                            (unsigned short)b_maxval));
      if ((x > 0) && (x >= (int)((unsigned int)(unsigned short)b_maxval >> 1) +
                               ((unsigned short)b_maxval & 1))) {
        q++;
      }
      data3_short[k] = (short)q;
      if (i1 < 0) {
        data3_short[k] = (short)-(short)q;
      }
    }
    b_data1_short[k] = data1_short[k];
  }
  fft2(b_data1_short, Data1_fft);
  xoffset = -1;
  for (j = 0; j < 64; j++) {
    xoffset++;
    for (k = 0; k < 16; k++) {
      nz = xoffset + (k << 6);
      tmp_re = Data1_fft[nz].re;
      tmp_im = Data1_fft[nz].im;
      Data1_fft[nz] = Data1_fft[nz + 1024];
      Data1_fft[nz + 1024].re = tmp_re;
      Data1_fft[nz + 1024].im = tmp_im;
    }
  }
  for (i = 0; i < 2048; i++) {
    b_data1_short[i] = data2_short[i];
  }
  fft2(b_data1_short, Data2_fft);
  xoffset = -1;
  for (j = 0; j < 64; j++) {
    xoffset++;
    for (k = 0; k < 16; k++) {
      nz = xoffset + (k << 6);
      tmp_re = Data2_fft[nz].re;
      tmp_im = Data2_fft[nz].im;
      Data2_fft[nz] = Data2_fft[nz + 1024];
      Data2_fft[nz + 1024].re = tmp_re;
      Data2_fft[nz + 1024].im = tmp_im;
    }
  }
  for (i = 0; i < 2048; i++) {
    b_data1_short[i] = data3_short[i];
  }
  fft2(b_data1_short, Data3_fft);
  xoffset = -1;
  for (j = 0; j < 64; j++) {
    xoffset++;
    for (k = 0; k < 16; k++) {
      nz = xoffset + (k << 6);
      tmp_re = Data3_fft[nz].re;
      tmp_im = Data3_fft[nz].im;
      Data3_fft[nz] = Data3_fft[nz + 1024];
      Data3_fft[nz + 1024].re = tmp_re;
      Data3_fft[nz + 1024].im = tmp_im;
    }
  }
  /*       */
  /*      % 取单边谱 */
  /*      [V, index] = max(Data1_fft); */
  /*   */
  /*  %     clear data1_short data2_short data3_short; */
  /*  %     clear Data1 Data2 Data3 Data1_fft Data2_fft Data3_fft; */
  /*      [flag] = start_judge(Data1_fft); */
  /*   */
  /*      if flag == true */
  /*      % 非相参积累 */
  for (k = 0; k < 1024; k++) {
    xoffset = k % 32 + ((k / 32) << 6);
    c_Data[k] = ((rt_hypotf_snf(Data1_fft[xoffset].re, Data1_fft[xoffset].im) +
                  rt_hypotf_snf(Data2_fft[xoffset].re, Data2_fft[xoffset].im)) +
                 rt_hypotf_snf(Data3_fft[xoffset].re, Data3_fft[xoffset].im)) /
                3.0F;
  }
  /*      Data = abs(Data1_fft); */
  /*      % 估计噪声水平并设置阈值 */
  tmp_re = c_Data[0];
  for (k = 0; k < 1023; k++) {
    tmp_re += c_Data[k + 1];
  }
  /*  */
  threshold = 10.0F * (tmp_re / 1024.0F);
  /*  将阈值设为噪声水平的3倍 */
  /*         %% 加权多普勒均值 */
  for (nz = 0; nz < 32; nz++) {
    xoffset = nz << 5;
    tmp_re = c_Data[xoffset];
    for (k = 0; k < 31; k++) {
      tmp_re += c_Data[(xoffset + k) + 1];
    }
    Zi_tmp[nz] = tmp_re;
  }
  /* 1x32 能量 */
  /* 多普勒索引转换为真正的速度 */
  /*  Di=96:127; */
  b_fcnOutput = Zi_tmp[0];
  for (k = 0; k < 31; k++) {
    b_fcnOutput += Zi_tmp[k + 1];
  }
  tmp_re = 0.0F;
  for (i = 0; i < 32; i++) {
    tmp_re += a[i] * Zi_tmp[i];
  }
  V = tmp_re / b_fcnOutput;
  /*         %% 选择热图中具有最高信号的N个像元 */
  /*  选择最高信号的5个像元 */
  /*  获取非相干积累结果的绝对值 */
  /*  % 获取热图中信号强度最高的N个像元的索引 */
  for (k = 0; k < 1024; k++) {
    tmp_re = c_Data[k];
    noncoherent_values[k] = tmp_re;
    tmp_re = roundf(tmp_re);
    if (tmp_re < 32768.0F) {
      i1 = (short)tmp_re;
    } else if (tmp_re >= 32768.0F) {
      i1 = MAX_int16_T;
    } else {
      i1 = 0;
    }
    b_x[k] = i1;
  }
  sort(b_x, iidx);
  /*  对信号强度排序 */
  /*  选择排序后信号强度最高的N个像素的索引 */
  /*  将一维索引转换为二维索引 */
  for (i = 0; i < 10; i++) {
    doppler_bins[i] = (float)iidx[i];
  }
  ind2sub_indexClass(doppler_bins, varargout_3, varargout_4);
  for (xoffset = 0; xoffset < 10; xoffset++) {
    doppler_bins[xoffset] = (float)varargout_4[xoffset];
    range_bins[xoffset] = (float)varargout_3[xoffset];
  }
  /*  计算角度FFT并提取高程和方位角加权平均值 */
  emxInit_real32_T(&elevation_angles, 2);
  elevation_angles_data = elevation_angles->data;
  elevation_angles->size[0] = 1;
  elevation_angles->size[1] = 0;
  emxInit_real32_T(&azimuth_angles, 2);
  azimuth_angles_data = azimuth_angles->data;
  azimuth_angles->size[0] = 1;
  azimuth_angles->size[1] = 0;
  emxInit_real32_T(&weights, 1);
  weights_data = weights->data;
  weights->size[0] = 0;
  for (j = 0; j < 10; j++) {
    creal32_T angle_spectrum_3_1[3];
    creal32_T pixel_data[3];
    creal32_T b_pixel_data[2];
    float c_fcnOutput[3];
    /*  提取对应像元的数据 */
    xj = (int)doppler_bins[j] - 1;
    k = (int)range_bins[j];
    xoffset = (k + (xj << 6)) - 1;
    pixel_data[2] = Data3_fft[xoffset];
    /*  计算角度FFT */
    b_pixel_data[0] = Data1_fft[xoffset];
    b_pixel_data[1] = pixel_data[2];
    fft(b_pixel_data, angle_spectrum_3_1);
    tmp_re = angle_spectrum_3_1[1].re;
    tmp_im = angle_spectrum_3_1[1].im;
    angle_spectrum_3_1[1] = angle_spectrum_3_1[0];
    angle_spectrum_3_1[0] = angle_spectrum_3_1[2];
    angle_spectrum_3_1[2].re = tmp_re;
    angle_spectrum_3_1[2].im = tmp_im;
    /*  水平对 */
    b_pixel_data[0] = Data2_fft[xoffset];
    b_pixel_data[1] = pixel_data[2];
    fft(b_pixel_data, pixel_data);
    tmp_re = pixel_data[1].re;
    tmp_im = pixel_data[1].im;
    pixel_data[1] = pixel_data[0];
    pixel_data[0] = pixel_data[2];
    /*  垂直对 */
    c_fcnOutput[0] =
        rt_hypotf_snf(angle_spectrum_3_1[0].re, angle_spectrum_3_1[0].im);
    c_fcnOutput[1] =
        rt_hypotf_snf(angle_spectrum_3_1[1].re, angle_spectrum_3_1[1].im);
    c_fcnOutput[2] =
        rt_hypotf_snf(angle_spectrum_3_1[2].re, angle_spectrum_3_1[2].im);
    maximum(c_fcnOutput, &ex, &xoffset);
    /*  水平对最大值索引 */
    c_fcnOutput[0] = rt_hypotf_snf(pixel_data[0].re, pixel_data[0].im);
    c_fcnOutput[1] = rt_hypotf_snf(pixel_data[1].re, pixel_data[1].im);
    c_fcnOutput[2] = rt_hypotf_snf(tmp_re, tmp_im);
    maximum(c_fcnOutput, &ex, &nz);
    /*  垂直对最大值索引 */
    /*  水平角度 */
    /*  俯仰角度 */
    /*  存储结果 */
    i = elevation_angles->size[1];
    i2 = elevation_angles->size[0] * elevation_angles->size[1];
    elevation_angles->size[1]++;
    emxEnsureCapacity_real32_T(elevation_angles, i2);
    elevation_angles_data = elevation_angles->data;
    elevation_angles_data[i] = ((float)nz - 1.0F) * 90.0F - 90.0F;
    i = azimuth_angles->size[1];
    i2 = azimuth_angles->size[0] * azimuth_angles->size[1];
    azimuth_angles->size[1]++;
    emxEnsureCapacity_real32_T(azimuth_angles, i2);
    azimuth_angles_data = azimuth_angles->data;
    azimuth_angles_data[i] = ((float)xoffset - 1.0F) * 90.0F - 90.0F;
    i = weights->size[0];
    i2 = weights->size[0];
    weights->size[0]++;
    emxEnsureCapacity_real32_T(weights, i2);
    weights_data = weights->data;
    weights_data[i] = noncoherent_values[(k + (xj << 5)) - 1];
  }
  /*  计算高程和方位角加权平均值 */
  y_tmp = sum(weights);
  tmp_re = 0.0F;
  xoffset = azimuth_angles->size[1];
  for (i = 0; i < xoffset; i++) {
    tmp_re += azimuth_angles_data[i] * weights_data[i];
  }
  emxFree_real32_T(&azimuth_angles);
  weighted_average_azimuth = tmp_re / y_tmp;
  /*       检测到的点数 */
  for (i = 0; i < 1024; i++) {
    c_x[i] = (noncoherent_values[i] > threshold);
  }
  nz = c_x[0];
  for (k = 0; k < 1023; k++) {
    nz += c_x[k + 1];
  }
  /*  计算范围平均值 */
  /*  距离索引 */
  memcpy(&d_fcnOutput[0], &c_Data[0], 32U * sizeof(float));
  for (k = 0; k < 31; k++) {
    xoffset = (k + 1) << 5;
    for (xj = 0; xj < 32; xj++) {
      d_fcnOutput[xj] += c_Data[xoffset + xj];
    }
  }
  for (i = 0; i < 32; i++) {
    d_fcnOutput[i] *= (float)i + 1.0F;
  }
  b_fcnOutput = d_fcnOutput[0];
  tmp_re = Zi_tmp[0];
  for (k = 0; k < 31; k++) {
    b_fcnOutput += d_fcnOutput[k + 1];
    tmp_re += Zi_tmp[k + 1];
  }
  /*  计算多普勒方位角相关性 */
  if (qY == 1U) {
    tmp_im = 0.0F;
  } else {
    if ((!rtIsInfF(V)) && (!rtIsNaNF(V))) {
      tmp_im = 0.0F;
    } else {
      tmp_im = rtNaNF;
    }
    if ((!rtIsInfF(weighted_average_azimuth)) &&
        (!rtIsNaNF(weighted_average_azimuth))) {
      threshold = 0.0F;
    } else {
      threshold = rtNaNF;
    }
    tmp_im = (V - V) * (weighted_average_azimuth - weighted_average_azimuth) /
             (tmp_im * threshold);
  }
  /*  将特征存储在矩阵中，横轴为帧数纵轴为特征 */
  ex = 0.0F;
  xoffset = elevation_angles->size[1];
  for (i = 0; i < xoffset; i++) {
    ex += elevation_angles_data[i] * weights_data[i];
  }
  emxFree_real32_T(&weights);
  emxFree_real32_T(&elevation_angles);
  featuresMatrix[0] = V;
  featuresMatrix[1] = ex / y_tmp;
  featuresMatrix[2] = weighted_average_azimuth;
  featuresMatrix[3] = (float)nz;
  featuresMatrix[4] = b_fcnOutput / tmp_re;
  featuresMatrix[5] = tmp_im;
  /*          meanBuffer(:,1,:) = buffer1; */
  /*          meanBuffer(:,2,:) = buffer2; */
  /*          meanBuffer(:,3,:) = buffer3; */
}

/*
 * File trailer for matlab_feature_extract.c
 *
 * [EOF]
 */
