/*
 * File: fft.c
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "modwt1.h"
#include "fft.h"
#include "modwt1_emxutil.h"
#include "fft1.h"
#include "bluesteinSetup.h"

/* Function Definitions */

/*
 * Arguments    : const emxArray_real_T *x
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void b_fft(const emxArray_real_T *x, emxArray_creal_T *y)
{
  emxArray_real_T *costab;
  emxArray_real_T *sintab;
  emxArray_real_T *sintabinv;
  int n1;
  boolean_T useRadix2;
  int ju;
  int sz_idx_0;
  emxArray_creal_T *b_y1;
  emxArray_creal_T *wwc;
  int j;
  int nRowsD2;
  int minNrowsNx;
  int nRowsD4;
  int iy;
  int i;
  double twid_re;
  double twid_im;
  emxArray_creal_T *fv;
  emxArray_creal_T *r1;
  emxArray_creal_T *b_fv;
  double temp_re;
  double temp_im;
  double fv_re;
  double fv_im;
  double wwc_im;
  double b_fv_re;
  emxInit_real_T(&costab, 2);
  emxInit_real_T(&sintab, 2);
  emxInit_real_T(&sintabinv, 2);
  n1 = x->size[1];
  useRadix2 = ((x->size[1] & (x->size[1] - 1)) == 0);
  get_algo_sizes(x->size[1], useRadix2, &ju, &sz_idx_0);
  generate_twiddle_tables(sz_idx_0, useRadix2, costab, sintab, sintabinv);
  emxInit_creal_T1(&b_y1, 1);
  if (useRadix2) {
    j = x->size[1] - 1;
    nRowsD2 = x->size[1] / 2;
    nRowsD4 = nRowsD2 / 2;
    sz_idx_0 = x->size[1];
    iy = b_y1->size[0];
    b_y1->size[0] = sz_idx_0;
    emxEnsureCapacity_creal_T1(b_y1, iy);
    sz_idx_0 = 0;
    ju = 0;
    iy = 0;
    for (i = 1; i <= j; i++) {
      b_y1->data[iy].re = x->data[sz_idx_0];
      b_y1->data[iy].im = 0.0;
      minNrowsNx = n1;
      useRadix2 = true;
      while (useRadix2) {
        minNrowsNx >>= 1;
        ju ^= minNrowsNx;
        useRadix2 = ((ju & minNrowsNx) == 0);
      }

      iy = ju;
      sz_idx_0++;
    }

    b_y1->data[iy].re = x->data[sz_idx_0];
    b_y1->data[iy].im = 0.0;
    for (i = 0; i <= n1 - 2; i += 2) {
      temp_re = b_y1->data[i + 1].re;
      temp_im = b_y1->data[i + 1].im;
      b_y1->data[i + 1].re = b_y1->data[i].re - b_y1->data[i + 1].re;
      b_y1->data[i + 1].im = b_y1->data[i].im - b_y1->data[i + 1].im;
      b_y1->data[i].re += temp_re;
      b_y1->data[i].im += temp_im;
    }

    minNrowsNx = 2;
    sz_idx_0 = 4;
    iy = 1 + ((nRowsD4 - 1) << 2);
    while (nRowsD4 > 0) {
      for (i = 0; i < iy; i += sz_idx_0) {
        temp_re = b_y1->data[i + minNrowsNx].re;
        temp_im = b_y1->data[i + minNrowsNx].im;
        b_y1->data[i + minNrowsNx].re = b_y1->data[i].re - temp_re;
        b_y1->data[i + minNrowsNx].im = b_y1->data[i].im - temp_im;
        b_y1->data[i].re += temp_re;
        b_y1->data[i].im += temp_im;
      }

      ju = 1;
      for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
        twid_re = costab->data[j];
        twid_im = sintab->data[j];
        i = ju;
        n1 = ju + iy;
        while (i < n1) {
          temp_re = twid_re * b_y1->data[i + minNrowsNx].re - twid_im *
            b_y1->data[i + minNrowsNx].im;
          temp_im = twid_re * b_y1->data[i + minNrowsNx].im + twid_im *
            b_y1->data[i + minNrowsNx].re;
          b_y1->data[i + minNrowsNx].re = b_y1->data[i].re - temp_re;
          b_y1->data[i + minNrowsNx].im = b_y1->data[i].im - temp_im;
          b_y1->data[i].re += temp_re;
          b_y1->data[i].im += temp_im;
          i += sz_idx_0;
        }

        ju++;
      }

      nRowsD4 /= 2;
      minNrowsNx = sz_idx_0;
      sz_idx_0 += sz_idx_0;
      iy -= minNrowsNx;
    }
  } else {
    emxInit_creal_T1(&wwc, 1);
    bluesteinSetup(x->size[1], wwc);
    minNrowsNx = x->size[1];
    sz_idx_0 = x->size[1];
    iy = b_y1->size[0];
    b_y1->size[0] = sz_idx_0;
    emxEnsureCapacity_creal_T1(b_y1, iy);
    sz_idx_0 = 0;
    for (iy = 0; iy < minNrowsNx; iy++) {
      twid_re = wwc->data[(n1 + iy) - 1].re;
      twid_im = wwc->data[(n1 + iy) - 1].im;
      b_y1->data[iy].re = twid_re * x->data[sz_idx_0];
      b_y1->data[iy].im = twid_im * -x->data[sz_idx_0];
      sz_idx_0++;
    }

    while (minNrowsNx + 1 <= n1) {
      b_y1->data[minNrowsNx].re = 0.0;
      b_y1->data[minNrowsNx].im = 0.0;
      minNrowsNx++;
    }

    emxInit_creal_T1(&fv, 1);
    emxInit_creal_T1(&r1, 1);
    emxInit_creal_T1(&b_fv, 1);
    r2br_r2dit_trig_impl(b_y1, ju, costab, sintab, fv);
    b_r2br_r2dit_trig_impl(wwc, ju, costab, sintab, r1);
    iy = b_fv->size[0];
    b_fv->size[0] = fv->size[0];
    emxEnsureCapacity_creal_T1(b_fv, iy);
    sz_idx_0 = fv->size[0];
    for (iy = 0; iy < sz_idx_0; iy++) {
      fv_re = fv->data[iy].re;
      fv_im = fv->data[iy].im;
      twid_re = r1->data[iy].re;
      twid_im = r1->data[iy].im;
      b_fv->data[iy].re = fv_re * twid_re - fv_im * twid_im;
      b_fv->data[iy].im = fv_re * twid_im + fv_im * twid_re;
    }

    emxFree_creal_T(&r1);
    r2br_r2dit_trig(b_fv, ju, costab, sintabinv, fv);
    sz_idx_0 = 0;
    iy = x->size[1] - 1;
    emxFree_creal_T(&b_fv);
    while (iy + 1 <= wwc->size[0]) {
      twid_re = wwc->data[iy].re;
      fv_re = fv->data[iy].re;
      twid_im = wwc->data[iy].im;
      fv_im = fv->data[iy].im;
      temp_re = wwc->data[iy].re;
      temp_im = fv->data[iy].im;
      wwc_im = wwc->data[iy].im;
      b_fv_re = fv->data[iy].re;
      b_y1->data[sz_idx_0].re = twid_re * fv_re + twid_im * fv_im;
      b_y1->data[sz_idx_0].im = temp_re * temp_im - wwc_im * b_fv_re;
      sz_idx_0++;
      iy++;
    }

    emxFree_creal_T(&fv);
    emxFree_creal_T(&wwc);
  }

  emxFree_real_T(&sintabinv);
  emxFree_real_T(&sintab);
  emxFree_real_T(&costab);
  iy = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity_creal_T(y, iy);
  sz_idx_0 = x->size[1];
  for (iy = 0; iy < sz_idx_0; iy++) {
    y->data[iy] = b_y1->data[iy];
  }

  emxFree_creal_T(&b_y1);
}

/*
 * Arguments    : const double x[8]
 *                double varargin_1
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void fft(const double x[8], double varargin_1, emxArray_creal_T *y)
{
  emxArray_real_T *costab;
  emxArray_real_T *sintab;
  emxArray_real_T *sintabinv;
  boolean_T useRadix2;
  int ju;
  int xidx;
  emxArray_creal_T *b_y1;
  emxArray_creal_T *wwc;
  int j;
  int minNrowsNx;
  int nRowsD2;
  int nRowsD4;
  int iheight;
  int i;
  double twid_re;
  double twid_im;
  emxArray_creal_T *fv;
  emxArray_creal_T *r0;
  emxArray_creal_T *b_fv;
  double temp_re;
  double temp_im;
  double fv_re;
  double fv_im;
  int ihi;
  double wwc_im;
  double b_fv_re;
  emxInit_real_T(&costab, 2);
  emxInit_real_T(&sintab, 2);
  emxInit_real_T(&sintabinv, 2);
  useRadix2 = (((int)varargin_1 & ((int)varargin_1 - 1)) == 0);
  get_algo_sizes((int)varargin_1, useRadix2, &ju, &xidx);
  generate_twiddle_tables(xidx, useRadix2, costab, sintab, sintabinv);
  emxInit_creal_T1(&b_y1, 1);
  if (useRadix2) {
    j = (int)varargin_1;
    if (8 < j) {
      j = 8;
    }

    nRowsD2 = (int)varargin_1 / 2;
    nRowsD4 = nRowsD2 / 2;
    xidx = b_y1->size[0];
    b_y1->size[0] = (int)varargin_1;
    emxEnsureCapacity_creal_T1(b_y1, xidx);
    if ((int)varargin_1 > 8) {
      iheight = b_y1->size[0];
      xidx = b_y1->size[0];
      b_y1->size[0] = iheight;
      emxEnsureCapacity_creal_T1(b_y1, xidx);
      for (xidx = 0; xidx < iheight; xidx++) {
        b_y1->data[xidx].re = 0.0;
        b_y1->data[xidx].im = 0.0;
      }
    }

    minNrowsNx = 0;
    ju = 0;
    xidx = 0;
    for (i = 1; i < j; i++) {
      b_y1->data[xidx].re = x[minNrowsNx];
      b_y1->data[xidx].im = 0.0;
      xidx = (int)varargin_1;
      useRadix2 = true;
      while (useRadix2) {
        xidx >>= 1;
        ju ^= xidx;
        useRadix2 = ((ju & xidx) == 0);
      }

      xidx = ju;
      minNrowsNx++;
    }

    b_y1->data[xidx].re = x[minNrowsNx];
    b_y1->data[xidx].im = 0.0;
    for (i = 0; i <= (int)varargin_1 - 2; i += 2) {
      temp_re = b_y1->data[i + 1].re;
      temp_im = b_y1->data[i + 1].im;
      b_y1->data[i + 1].re = b_y1->data[i].re - b_y1->data[i + 1].re;
      b_y1->data[i + 1].im = b_y1->data[i].im - b_y1->data[i + 1].im;
      b_y1->data[i].re += temp_re;
      b_y1->data[i].im += temp_im;
    }

    xidx = 2;
    minNrowsNx = 4;
    iheight = 1 + ((nRowsD4 - 1) << 2);
    while (nRowsD4 > 0) {
      for (i = 0; i < iheight; i += minNrowsNx) {
        temp_re = b_y1->data[i + xidx].re;
        temp_im = b_y1->data[i + xidx].im;
        b_y1->data[i + xidx].re = b_y1->data[i].re - temp_re;
        b_y1->data[i + xidx].im = b_y1->data[i].im - temp_im;
        b_y1->data[i].re += temp_re;
        b_y1->data[i].im += temp_im;
      }

      ju = 1;
      for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
        twid_re = costab->data[j];
        twid_im = sintab->data[j];
        i = ju;
        ihi = ju + iheight;
        while (i < ihi) {
          temp_re = twid_re * b_y1->data[i + xidx].re - twid_im * b_y1->data[i +
            xidx].im;
          temp_im = twid_re * b_y1->data[i + xidx].im + twid_im * b_y1->data[i +
            xidx].re;
          b_y1->data[i + xidx].re = b_y1->data[i].re - temp_re;
          b_y1->data[i + xidx].im = b_y1->data[i].im - temp_im;
          b_y1->data[i].re += temp_re;
          b_y1->data[i].im += temp_im;
          i += minNrowsNx;
        }

        ju++;
      }

      nRowsD4 /= 2;
      xidx = minNrowsNx;
      minNrowsNx += minNrowsNx;
      iheight -= xidx;
    }
  } else {
    emxInit_creal_T1(&wwc, 1);
    bluesteinSetup((int)varargin_1, wwc);
    minNrowsNx = (int)varargin_1;
    if (!(minNrowsNx < 8)) {
      minNrowsNx = 8;
    }

    xidx = b_y1->size[0];
    b_y1->size[0] = (int)varargin_1;
    emxEnsureCapacity_creal_T1(b_y1, xidx);
    if ((int)varargin_1 > 8) {
      iheight = b_y1->size[0];
      xidx = b_y1->size[0];
      b_y1->size[0] = iheight;
      emxEnsureCapacity_creal_T1(b_y1, xidx);
      for (xidx = 0; xidx < iheight; xidx++) {
        b_y1->data[xidx].re = 0.0;
        b_y1->data[xidx].im = 0.0;
      }
    }

    xidx = 0;
    for (iheight = 0; iheight < minNrowsNx; iheight++) {
      twid_re = wwc->data[((int)varargin_1 + iheight) - 1].re;
      twid_im = wwc->data[((int)varargin_1 + iheight) - 1].im;
      b_y1->data[iheight].re = twid_re * x[xidx];
      b_y1->data[iheight].im = twid_im * -x[xidx];
      xidx++;
    }

    while (minNrowsNx + 1 <= (int)varargin_1) {
      b_y1->data[minNrowsNx].re = 0.0;
      b_y1->data[minNrowsNx].im = 0.0;
      minNrowsNx++;
    }

    emxInit_creal_T1(&fv, 1);
    emxInit_creal_T1(&r0, 1);
    emxInit_creal_T1(&b_fv, 1);
    r2br_r2dit_trig_impl(b_y1, ju, costab, sintab, fv);
    b_r2br_r2dit_trig_impl(wwc, ju, costab, sintab, r0);
    xidx = b_fv->size[0];
    b_fv->size[0] = fv->size[0];
    emxEnsureCapacity_creal_T1(b_fv, xidx);
    iheight = fv->size[0];
    for (xidx = 0; xidx < iheight; xidx++) {
      fv_re = fv->data[xidx].re;
      fv_im = fv->data[xidx].im;
      twid_re = r0->data[xidx].re;
      twid_im = r0->data[xidx].im;
      b_fv->data[xidx].re = fv_re * twid_re - fv_im * twid_im;
      b_fv->data[xidx].im = fv_re * twid_im + fv_im * twid_re;
    }

    emxFree_creal_T(&r0);
    r2br_r2dit_trig(b_fv, ju, costab, sintabinv, fv);
    xidx = 0;
    iheight = (int)varargin_1 - 1;
    emxFree_creal_T(&b_fv);
    while (iheight + 1 <= wwc->size[0]) {
      twid_re = wwc->data[iheight].re;
      fv_re = fv->data[iheight].re;
      twid_im = wwc->data[iheight].im;
      fv_im = fv->data[iheight].im;
      temp_re = wwc->data[iheight].re;
      temp_im = fv->data[iheight].im;
      wwc_im = wwc->data[iheight].im;
      b_fv_re = fv->data[iheight].re;
      b_y1->data[xidx].re = twid_re * fv_re + twid_im * fv_im;
      b_y1->data[xidx].im = temp_re * temp_im - wwc_im * b_fv_re;
      xidx++;
      iheight++;
    }

    emxFree_creal_T(&fv);
    emxFree_creal_T(&wwc);
  }

  emxFree_real_T(&sintabinv);
  emxFree_real_T(&sintab);
  emxFree_real_T(&costab);
  xidx = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = (int)varargin_1;
  emxEnsureCapacity_creal_T(y, xidx);
  iheight = (int)varargin_1;
  for (xidx = 0; xidx < iheight; xidx++) {
    y->data[xidx] = b_y1->data[xidx];
  }

  emxFree_creal_T(&b_y1);
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */
