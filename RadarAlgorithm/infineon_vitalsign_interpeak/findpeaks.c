/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findpeaks.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 28-Feb-2023 17:18:31
 */

/* Include Files */
#include "findpeaks.h"
#include "eml_setop.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const float Yin[256]
 *                float Ypk_data[]
 *                int *Ypk_size
 *                double Xpk_data[]
 *                int *Xpk_size
 * Return Type  : void
 */
void findpeaks(const float Yin[256], float Ypk_data[], int *Ypk_size,
               double Xpk_data[], int *Xpk_size)
{
  float yk;
  float ykfirst;
  int b_iPk_data[512];
  int c_data[512];
  int idx_data[512];
  int iFinite_data[256];
  int iInfinite_data[256];
  int iInflect_data[256];
  int iPk_data[256];
  int b_nPk;
  int k;
  int kfirst;
  int nInf;
  int nPk;
  char dir;
  char previousdir;
  boolean_T isinfyk;
  boolean_T isinfykfirst;
  nPk = -1;
  nInf = -1;
  dir = 'n';
  kfirst = -1;
  ykfirst = rtInfF;
  isinfykfirst = true;
  for (k = 0; k < 256; k++) {
    yk = Yin[k];
    if (rtIsNaNF(yk)) {
      yk = rtInfF;
      isinfyk = true;
    } else if (rtIsInfF(yk) && (yk > 0.0F)) {
      isinfyk = true;
      nInf++;
      iInfinite_data[nInf] = k + 1;
    } else {
      isinfyk = false;
    }
    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
      } else if (yk < ykfirst) {
        dir = 'd';
        if (('d' != previousdir) && (previousdir == 'i')) {
          nPk++;
          iFinite_data[nPk] = kfirst + 1;
        }
      } else {
        dir = 'i';
      }
      ykfirst = yk;
      kfirst = k;
      isinfykfirst = isinfyk;
    }
  }
  b_nPk = 0;
  if (1 > nPk + 1) {
    nPk = -1;
  }
  kfirst = nPk + 1;
  for (k = 0; k < kfirst; k++) {
    nPk = iFinite_data[k];
    yk = Yin[nPk - 1];
    if (yk > rtMinusInfF) {
      ykfirst = Yin[nPk];
      if ((Yin[nPk - 2] > ykfirst) || rtIsNaNF(ykfirst)) {
        ykfirst = Yin[nPk - 2];
      } else {
        ykfirst = Yin[nPk];
      }
      if (yk - ykfirst >= 0.0F) {
        b_nPk++;
        iPk_data[b_nPk - 1] = nPk;
      }
    }
  }
  if (1 > b_nPk) {
    b_nPk = 0;
  }
  if (1 > nInf + 1) {
    nInf = -1;
  }
  do_vectors(iPk_data, b_nPk, iInfinite_data, nInf + 1, c_data, &kfirst,
             iInflect_data, &nPk, iFinite_data, &k);
  if (kfirst < 1) {
    *Ypk_size = 0;
  } else {
    *Ypk_size = kfirst;
  }
  if (*Ypk_size > 0) {
    b_iPk_data[0] = 1;
    kfirst = 1;
    for (k = 2; k <= *Ypk_size; k++) {
      kfirst++;
      b_iPk_data[k - 1] = kfirst;
    }
  }
  if (0 <= *Ypk_size - 1) {
    memcpy(&idx_data[0], &b_iPk_data[0], *Ypk_size * sizeof(int));
  }
  if (*Ypk_size > 256) {
    *Ypk_size = 256;
  }
  for (nPk = 0; nPk < *Ypk_size; nPk++) {
    b_iPk_data[nPk] = c_data[idx_data[nPk] - 1];
  }
  for (nPk = 0; nPk < *Ypk_size; nPk++) {
    Ypk_data[nPk] = Yin[b_iPk_data[nPk] - 1];
  }
  *Xpk_size = *Ypk_size;
  for (nPk = 0; nPk < *Ypk_size; nPk++) {
    Xpk_data[nPk] = (short)((short)(b_iPk_data[nPk] - 1) + 1);
  }
}

/*
 * File trailer for findpeaks.c
 *
 * [EOF]
 */
