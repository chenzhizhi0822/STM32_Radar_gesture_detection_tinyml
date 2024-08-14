/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: start_judge.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 14:10:44
 */

/* Include Files */
#include "start_judge.h"
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static float rt_hypotf_snf(float u0, float u1);

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
 * 64x32
 *  disp("------开始检测有无手势------")
 * tic
 *
 * Arguments    : const unsigned short Data[2048]
 * Return Type  : bool
 */
bool start_judge(const unsigned short Data[2048])
{
  static const float fv[32] = {
      0.08F,         0.0894162282F, 0.11727941F,  0.162448823F, 0.223075211F,
      0.296676546F,  0.380239576F,  0.470343232F, 0.563298643F, 0.65530014F,
      0.742581308F,  0.821568727F,  0.889028728F, 0.942199469F, 0.978904068F,
      0.997639894F,  0.997639894F,  0.978904068F, 0.942199469F, 0.889028728F,
      0.821568727F,  0.742581308F,  0.65530014F,  0.563298643F, 0.470343232F,
      0.380239576F,  0.296676546F,  0.223075211F, 0.162448823F, 0.11727941F,
      0.0894162282F, 0.08F};
  static const float fv1[17] = {
      1.0F,          0.980785251F,  0.923879504F,  0.831469595F,  0.707106769F,
      0.555570245F,  0.382683456F,  0.195090324F,  0.0F,          -0.195090324F,
      -0.382683456F, -0.555570245F, -0.707106769F, -0.831469595F, -0.923879504F,
      -0.980785251F, -1.0F};
  static const float fv2[17] = {
      0.0F,          -0.195090324F, -0.382683456F, -0.555570245F, -0.707106769F,
      -0.831469595F, -0.923879504F, -0.980785251F, -1.0F,         -0.980785251F,
      -0.923879504F, -0.831469595F, -0.707106769F, -0.555570245F, -0.382683456F,
      -0.195090324F, -0.0F};
static  creal32_T tmp[2048];
static  creal32_T b_y1[1984];
static  creal32_T Data_range[1024];
static  creal32_T speed_profile[1024];
static  float x[2048];
  float range_frame[32];
  float speed_frame[32];
  float pink_range;
  float tmp2_im;
  float tmp2_re;
  float work_im;
  float work_re;
  int i;
  int iDelta2;
  int iheight;
  int ihi;
  int iy;
  int ju;
  int k;
  int lowerDim;
  int n;
  signed char b_ii_data[32];
  signed char ii_data[32];
  bool exitg1;
  bool flag;
  bool tst;
  /*  ----------------判断开始采集标志子函数 */
  /*  Data : 64*32, 64 ADC, 32 chirp */
  /*  英飞凌手势采集数据参数 */
  /*  Number of time samples per sweep */
  /*  number of chirps */
  /*  Reshape data into chirps and plot Range-Time */
  /*  Data_time=reshape(Data, [NTS nc]); */
  /* 矩形窗 */
  /*  距离fft */
  memset(&Data_range[0], 0, 1024U * sizeof(creal32_T));
  /* 做完diff后会少一列 */
  /* for i = 1:n_rx  %n_rx = 3 */
  for (iheight = 0; iheight < 2048; iheight++) {
    unsigned short u;
    u = Data[iheight];
    if (u > 32767) {
      u = 32767U;
    }
    x[iheight] = u;
  }
  for (lowerDim = 0; lowerDim < 32; lowerDim++) {
    c_FFTImplementationCallback_doH(x, lowerDim << 6,
                                    *(creal32_T(*)[64]) & tmp[lowerDim << 6]);
  }
  /* 消直流分量 */
  /*  tmp = tmp(:,1:128*64);  %ti数据 */
  iy = 0;
  for (ju = 0; ju < 64; ju++) {
    work_re = tmp[ju].re;
    work_im = tmp[ju].im;
    for (iDelta2 = 0; iDelta2 < 31; iDelta2++) {
      tmp2_re = work_re;
      tmp2_im = work_im;
      lowerDim = iDelta2 << 6;
      iheight = (ju + lowerDim) + 64;
      work_re = tmp[iheight].re;
      work_im = tmp[iheight].im;
      iheight = iy + lowerDim;
      b_y1[iheight].re = work_re - tmp2_re;
      b_y1[iheight].im = work_im - tmp2_im;
    }
    iy++;
  }
  for (iheight = 0; iheight < 31; iheight++) {
    memcpy(&Data_range[iheight * 32], &b_y1[iheight * 64],
           32U * sizeof(creal32_T));
  }
  /* end */
  /*  figure */
  /*  imagesc(abs(Data_range(:,:,1)));  %size(Data_range):32 1536  3 */
  /*  title('消除直流分量'); */
  /*  title("1Dfft") */
  /*  axis xy */
  /*  xlabel("time"); */
  /*  ylabel("range"); */
  /*  32,32,48,3 */
  /*  多普勒FFT */
  for (i = 0; i < 32; i++) {
    Data_range[i + 992] = Data_range[i + 960];
    speed_frame[i] = 0.0F;
    range_frame[i] = 0.0F;
  }
  for (n = 0; n < 32; n++) {
    creal32_T temp[32];
    creal32_T yCol[32];
    float work_re_tmp;
    int b_work_re_tmp;
    for (iheight = 0; iheight < 32; iheight++) {
      lowerDim = n + (iheight << 5);
      work_im = fv[iheight];
      temp[iheight].re = work_im * Data_range[lowerDim].re;
      temp[iheight].im = work_im * Data_range[lowerDim].im;
    }
    iy = 0;
    ju = 0;
    for (i = 0; i < 31; i++) {
      yCol[iy] = temp[i];
      lowerDim = 32;
      tst = true;
      while (tst) {
        lowerDim >>= 1;
        ju ^= lowerDim;
        tst = ((ju & lowerDim) == 0);
      }
      iy = ju;
    }
    yCol[iy] = temp[31];
    for (i = 0; i <= 30; i += 2) {
      work_re_tmp = yCol[i + 1].re;
      pink_range = yCol[i + 1].im;
      work_re = yCol[i].re;
      work_im = yCol[i].im;
      yCol[i + 1].re = work_re - work_re_tmp;
      yCol[i + 1].im = work_im - pink_range;
      yCol[i].re = work_re + work_re_tmp;
      yCol[i].im = work_im + pink_range;
    }
    ju = 2;
    iDelta2 = 4;
    k = 8;
    iheight = 29;
    while (k > 0) {
      for (i = 0; i < iheight; i += iDelta2) {
        b_work_re_tmp = i + ju;
        work_re = yCol[b_work_re_tmp].re;
        work_im = yCol[b_work_re_tmp].im;
        yCol[b_work_re_tmp].re = yCol[i].re - work_re;
        yCol[b_work_re_tmp].im = yCol[i].im - work_im;
        yCol[i].re += work_re;
        yCol[i].im += work_im;
      }
      lowerDim = 1;
      for (iy = k; iy < 16; iy += k) {
        tmp2_re = fv1[iy];
        tmp2_im = fv2[iy];
        i = lowerDim;
        ihi = lowerDim + iheight;
        while (i < ihi) {
          b_work_re_tmp = i + ju;
          work_re_tmp = yCol[b_work_re_tmp].im;
          pink_range = yCol[b_work_re_tmp].re;
          work_re = tmp2_re * pink_range - tmp2_im * work_re_tmp;
          work_im = tmp2_re * work_re_tmp + tmp2_im * pink_range;
          yCol[b_work_re_tmp].re = yCol[i].re - work_re;
          yCol[b_work_re_tmp].im = yCol[i].im - work_im;
          yCol[i].re += work_re;
          yCol[i].im += work_im;
          i += iDelta2;
        }
        lowerDim++;
      }
      k /= 2;
      ju = iDelta2;
      iDelta2 += iDelta2;
      iheight -= ju;
    }
    for (ihi = 0; ihi < 2; ihi++) {
      int a;
      a = 31 * ihi;
      iheight = 31 * ihi + 1;
      if (iheight > 1) {
        int npages;
        int vlend2;
        vlend2 = iheight / 2 - 1;
        npages = 1;
        lowerDim = ihi + 2;
        for (k = lowerDim; k < 3; k++) {
          npages <<= 5;
        }
        if ((vlend2 + 1) << 1 == iheight) {
          lowerDim = 0;
          for (i = 0; i < npages; i++) {
            iy = lowerDim;
            lowerDim += a;
            iy++;
            lowerDim++;
            iDelta2 = iy + vlend2;
            for (k = 0; k <= vlend2; k++) {
              b_work_re_tmp = (iy + k) - 1;
              work_re = yCol[b_work_re_tmp].re;
              work_im = yCol[b_work_re_tmp].im;
              iheight = iDelta2 + k;
              yCol[b_work_re_tmp] = yCol[iheight];
              yCol[iheight].re = work_re;
              yCol[iheight].im = work_im;
            }
          }
        } else {
          lowerDim = 0;
          for (i = 0; i < npages; i++) {
            iy = lowerDim;
            lowerDim += a;
            iy++;
            lowerDim++;
            iDelta2 = iy + vlend2;
            work_re = yCol[iDelta2].re;
            work_im = yCol[iDelta2].im;
            for (k = 0; k <= vlend2; k++) {
              iheight = iDelta2 + k;
              ju = (iy + k) - 1;
              yCol[iheight] = yCol[ju];
              yCol[ju] = yCol[iheight + 1];
            }
            iDelta2 = (iDelta2 + vlend2) + 1;
            yCol[iDelta2].re = work_re;
            yCol[iDelta2].im = work_im;
          }
        }
      }
    }
    /* 对rangeFFT结果进行n_chirps点FFT */
    for (iheight = 0; iheight < 32; iheight++) {
      lowerDim = iheight + (n << 5);
      speed_profile[lowerDim].re = yCol[iheight].re;
      speed_profile[lowerDim].im = -yCol[iheight].im;
    }
    /* velocity , range , frames */
  }
  /*  对speed_profile按chirp维非相参积累得到 距离-时间图（32*frame） */
  /*  对speed_profile按range维非相参积累得到 速度-时间图（32*frame） */
  for (n = 0; n < 32; n++) {
    for (k = 0; k < 32; k++) {
      iheight = n + (k << 5);
      range_frame[k] +=
          rt_hypotf_snf(speed_profile[iheight].re, speed_profile[iheight].im);
      iheight = k + (n << 5);
      speed_frame[k] +=
          rt_hypotf_snf(speed_profile[iheight].re, speed_profile[iheight].im);
    }
  }
  if (!rtIsNaNF(range_frame[0])) {
    ju = 1;
  } else {
    ju = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 33)) {
      if (!rtIsNaNF(range_frame[k - 1])) {
        ju = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (ju == 0) {
    pink_range = range_frame[0];
  } else {
    pink_range = range_frame[ju - 1];
    iheight = ju + 1;
    for (k = iheight; k < 33; k++) {
      work_im = range_frame[k - 1];
      if (pink_range < work_im) {
        pink_range = work_im;
      }
    }
  }
  if (!rtIsNaNF(speed_frame[0])) {
    ju = 1;
  } else {
    ju = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 33)) {
      if (!rtIsNaNF(speed_frame[k - 1])) {
        ju = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (ju == 0) {
    work_re = speed_frame[0];
  } else {
    work_re = speed_frame[ju - 1];
    iheight = ju + 1;
    for (k = iheight; k < 33; k++) {
      work_im = speed_frame[k - 1];
      if (work_re < work_im) {
        work_re = work_im;
      }
    }
  }
  ju = 0;
  lowerDim = 0;
  exitg1 = false;
  while ((!exitg1) && (lowerDim < 32)) {
    if (range_frame[lowerDim] == pink_range) {
      ju++;
      ii_data[ju - 1] = (signed char)(lowerDim + 1);
      if (ju >= 32) {
        exitg1 = true;
      } else {
        lowerDim++;
      }
    } else {
      lowerDim++;
    }
  }
  if (ju < 1) {
    iDelta2 = 0;
  } else {
    iDelta2 = ju;
  }
  /* 将线性索引转换为下标 */
  ju = 0;
  lowerDim = 0;
  exitg1 = false;
  while ((!exitg1) && (lowerDim < 32)) {
    if (speed_frame[lowerDim] == work_re) {
      ju++;
      b_ii_data[ju - 1] = (signed char)(lowerDim + 1);
      if (ju >= 32) {
        exitg1 = true;
      } else {
        lowerDim++;
      }
    } else {
      lowerDim++;
    }
  }
  if (ju < 1) {
    lowerDim = 0;
  } else {
    lowerDim = ju;
  }
  for (iheight = 0; iheight < lowerDim; iheight++) {
    b_ii_data[iheight]--;
  }
  for (iheight = 0; iheight < lowerDim; iheight++) {
    signed char b_i;
    b_i = b_ii_data[iheight];
    b_i = (signed char)(b_i - ((b_i / 32) << 5));
    b_ii_data[iheight] = b_i;
  }
  /*  48.75mm以内的动作产生，则开始采集。(不添加速度=0判断：慢速运动的左右、上下速度会解析为0)
   */
  /*  if pink_range>9500 & pink_speed>4500 & range_index>1 & range_index<14 &
   * speed_index~=16 */
  if ((pink_range > 9500.0F) && (work_re > 4500.0F)) {
    bool x_data[32];
    for (iheight = 0; iheight < iDelta2; iheight++) {
      x_data[iheight] = (ii_data[iheight] > 1);
    }
    tst = (iDelta2 != 0);
    if (tst) {
      k = 0;
      exitg1 = false;
      while ((!exitg1) && (k <= iDelta2 - 1)) {
        if (!x_data[k]) {
          tst = false;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (tst) {
      for (iheight = 0; iheight < iDelta2; iheight++) {
        x_data[iheight] = (ii_data[iheight] < 12);
      }
      tst = (iDelta2 != 0);
      if (tst) {
        k = 0;
        exitg1 = false;
        while ((!exitg1) && (k <= iDelta2 - 1)) {
          if (!x_data[k]) {
            tst = false;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }
      if (tst) {
        for (iheight = 0; iheight < lowerDim; iheight++) {
          x_data[iheight] = (b_ii_data[iheight] + 1 != 16);
        }
        tst = (lowerDim != 0);
        if (tst) {
          k = 0;
          exitg1 = false;
          while ((!exitg1) && (k <= lowerDim - 1)) {
            if (!x_data[k]) {
              tst = false;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (tst) {
          flag = true;
          /*      disp('------range_index') */
          /*      disp(range_index) */
        } else {
          flag = false;
        }
      } else {
        flag = false;
      }
    } else {
      flag = false;
    }
  } else {
    flag = false;
  }
  /* toc */
  /*  disp('-------检测结束--------') */
  return flag;
}

/*
 * File trailer for start_judge.c
 *
 * [EOF]
 */
