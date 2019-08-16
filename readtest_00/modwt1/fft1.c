/*
 * File: fft1.c
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

/* Include Files */
#include <math.h>
#include "rt_nonfinite.h"
#include "modwt1.h"
#include "fft1.h"
#include "modwt1_emxutil.h"

/* Function Definitions */

/*
 * Arguments    : const emxArray_creal_T *x
 *                int unsigned_nRows
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void b_r2br_r2dit_trig_impl(const emxArray_creal_T *x, int unsigned_nRows, const
  emxArray_real_T *costab, const emxArray_real_T *sintab, emxArray_creal_T *y)
{
  int j;
  int nRowsD2;
  int nRowsD4;
  int iy;
  int iDelta;
  int ix;
  int ju;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  double twid_re;
  double twid_im;
  int ihi;
  j = x->size[0];
  if (!(j < unsigned_nRows)) {
    j = unsigned_nRows;
  }

  nRowsD2 = unsigned_nRows / 2;
  nRowsD4 = nRowsD2 / 2;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal_T1(y, iy);
  if (unsigned_nRows > x->size[0]) {
    iDelta = y->size[0];
    iy = y->size[0];
    y->size[0] = iDelta;
    emxEnsureCapacity_creal_T1(y, iy);
    for (iy = 0; iy < iDelta; iy++) {
      y->data[iy].re = 0.0;
      y->data[iy].im = 0.0;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 1; i < j; i++) {
    y->data[iy] = x->data[ix];
    iDelta = unsigned_nRows;
    tst = true;
    while (tst) {
      iDelta >>= 1;
      ju ^= iDelta;
      tst = ((ju & iDelta) == 0);
    }

    iy = ju;
    ix++;
  }

  y->data[iy] = x->data[ix];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= unsigned_nRows - 2; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
  }

  iDelta = 2;
  iy = 4;
  ix = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ix; i += iy) {
      temp_re = y->data[i + iDelta].re;
      temp_im = y->data[i + iDelta].im;
      y->data[i + iDelta].re = y->data[i].re - temp_re;
      y->data[i + iDelta].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }

    ju = 1;
    for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      i = ju;
      ihi = ju + ix;
      while (i < ihi) {
        temp_re = twid_re * y->data[i + iDelta].re - twid_im * y->data[i +
          iDelta].im;
        temp_im = twid_re * y->data[i + iDelta].im + twid_im * y->data[i +
          iDelta].re;
        y->data[i + iDelta].re = y->data[i].re - temp_re;
        y->data[i + iDelta].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iy;
      }

      ju++;
    }

    nRowsD4 /= 2;
    iDelta = iy;
    iy += iy;
    ix -= iDelta;
  }
}

/*
 * Arguments    : const emxArray_creal_T *x
 *                int n
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void c_fft(const emxArray_creal_T *x, int n, emxArray_creal_T *y)
{
  emxArray_real_T *costab1q;
  boolean_T useRadix2;
  int N2blue;
  int nd2;
  double e;
  int nRowsD4;
  int nInt2;
  int k;
  emxArray_real_T *costab;
  emxArray_real_T *sintab;
  emxArray_real_T *sintabinv;
  emxArray_creal_T *wwc;
  int nInt2m1;
  int b_y;
  double denom_im;
  emxArray_creal_T *fv;
  double wwc_re;
  emxArray_creal_T *r3;
  double fv_im;
  emxArray_creal_T *b_fv;
  double wwc_im;
  double fv_re;
  double b_fv_re;
  double b_fv_im;
  emxInit_real_T(&costab1q, 2);
  useRadix2 = ((n & (n - 1)) == 0);
  get_algo_sizes(n, useRadix2, &N2blue, &nd2);
  e = 6.2831853071795862 / (double)nd2;
  nRowsD4 = nd2 / 2 / 2;
  nInt2 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = nRowsD4 + 1;
  emxEnsureCapacity_real_T(costab1q, nInt2);
  costab1q->data[0] = 1.0;
  nd2 = nRowsD4 / 2;
  for (k = 1; k <= nd2; k++) {
    costab1q->data[k] = cos(e * (double)k);
  }

  for (k = nd2 + 1; k < nRowsD4; k++) {
    costab1q->data[k] = sin(e * (double)(nRowsD4 - k));
  }

  costab1q->data[nRowsD4] = 0.0;
  emxInit_real_T(&costab, 2);
  emxInit_real_T(&sintab, 2);
  emxInit_real_T(&sintabinv, 2);
  if (!useRadix2) {
    nd2 = costab1q->size[1] - 1;
    nRowsD4 = (costab1q->size[1] - 1) << 1;
    nInt2 = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nRowsD4 + 1;
    emxEnsureCapacity_real_T(costab, nInt2);
    nInt2 = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nRowsD4 + 1;
    emxEnsureCapacity_real_T(sintab, nInt2);
    costab->data[0] = 1.0;
    sintab->data[0] = 0.0;
    nInt2 = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = nRowsD4 + 1;
    emxEnsureCapacity_real_T(sintabinv, nInt2);
    for (k = 1; k <= nd2; k++) {
      sintabinv->data[k] = costab1q->data[nd2 - k];
    }

    for (k = costab1q->size[1]; k <= nRowsD4; k++) {
      sintabinv->data[k] = costab1q->data[k - nd2];
    }

    for (k = 1; k <= nd2; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = -costab1q->data[nd2 - k];
    }

    for (k = costab1q->size[1]; k <= nRowsD4; k++) {
      costab->data[k] = -costab1q->data[nRowsD4 - k];
      sintab->data[k] = -costab1q->data[k - nd2];
    }
  } else {
    nd2 = costab1q->size[1] - 1;
    nRowsD4 = (costab1q->size[1] - 1) << 1;
    nInt2 = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = nRowsD4 + 1;
    emxEnsureCapacity_real_T(costab, nInt2);
    nInt2 = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = nRowsD4 + 1;
    emxEnsureCapacity_real_T(sintab, nInt2);
    costab->data[0] = 1.0;
    sintab->data[0] = 0.0;
    for (k = 1; k <= nd2; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = costab1q->data[nd2 - k];
    }

    for (k = costab1q->size[1]; k <= nRowsD4; k++) {
      costab->data[k] = -costab1q->data[nRowsD4 - k];
      sintab->data[k] = costab1q->data[k - nd2];
    }

    nInt2 = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
    emxEnsureCapacity_real_T(sintabinv, nInt2);
  }

  emxFree_real_T(&costab1q);
  if (useRadix2) {
    r2br_r2dit_trig(x, n, costab, sintab, y);
  } else {
    emxInit_creal_T1(&wwc, 1);
    nInt2m1 = (n + n) - 1;
    nInt2 = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal_T1(wwc, nInt2);
    nd2 = n;
    nRowsD4 = 0;
    wwc->data[n - 1].re = 1.0;
    wwc->data[n - 1].im = 0.0;
    nInt2 = n << 1;
    for (k = 1; k < n; k++) {
      b_y = (k << 1) - 1;
      if (nInt2 - nRowsD4 <= b_y) {
        nRowsD4 += b_y - nInt2;
      } else {
        nRowsD4 += b_y;
      }

      denom_im = 3.1415926535897931 * (double)nRowsD4 / (double)n;
      if (denom_im == 0.0) {
        e = 1.0;
        denom_im = 0.0;
      } else {
        e = cos(denom_im);
        denom_im = sin(denom_im);
      }

      wwc->data[nd2 - 2].re = e;
      wwc->data[nd2 - 2].im = -denom_im;
      nd2--;
    }

    nd2 = 0;
    for (k = nInt2m1 - 1; k >= n; k--) {
      wwc->data[k] = wwc->data[nd2];
      nd2++;
    }

    nRowsD4 = x->size[0];
    if (n < nRowsD4) {
      nRowsD4 = n;
    }

    nInt2 = y->size[0];
    y->size[0] = n;
    emxEnsureCapacity_creal_T1(y, nInt2);
    if (n > x->size[0]) {
      nd2 = y->size[0];
      nInt2 = y->size[0];
      y->size[0] = nd2;
      emxEnsureCapacity_creal_T1(y, nInt2);
      for (nInt2 = 0; nInt2 < nd2; nInt2++) {
        y->data[nInt2].re = 0.0;
        y->data[nInt2].im = 0.0;
      }
    }

    nd2 = 0;
    for (k = 0; k < nRowsD4; k++) {
      e = wwc->data[(n + k) - 1].re;
      denom_im = wwc->data[(n + k) - 1].im;
      wwc_re = x->data[nd2].re;
      fv_im = x->data[nd2].im;
      wwc_im = x->data[nd2].im;
      fv_re = x->data[nd2].re;
      y->data[k].re = e * wwc_re + denom_im * fv_im;
      y->data[k].im = e * wwc_im - denom_im * fv_re;
      nd2++;
    }

    while (nRowsD4 + 1 <= n) {
      y->data[nRowsD4].re = 0.0;
      y->data[nRowsD4].im = 0.0;
      nRowsD4++;
    }

    emxInit_creal_T1(&fv, 1);
    emxInit_creal_T1(&r3, 1);
    emxInit_creal_T1(&b_fv, 1);
    r2br_r2dit_trig_impl(y, N2blue, costab, sintab, fv);
    b_r2br_r2dit_trig_impl(wwc, N2blue, costab, sintab, r3);
    nInt2 = b_fv->size[0];
    b_fv->size[0] = fv->size[0];
    emxEnsureCapacity_creal_T1(b_fv, nInt2);
    nd2 = fv->size[0];
    for (nInt2 = 0; nInt2 < nd2; nInt2++) {
      b_fv_re = fv->data[nInt2].re;
      b_fv_im = fv->data[nInt2].im;
      e = r3->data[nInt2].re;
      denom_im = r3->data[nInt2].im;
      b_fv->data[nInt2].re = b_fv_re * e - b_fv_im * denom_im;
      b_fv->data[nInt2].im = b_fv_re * denom_im + b_fv_im * e;
    }

    emxFree_creal_T(&r3);
    r2br_r2dit_trig(b_fv, N2blue, costab, sintabinv, fv);
    nd2 = 0;
    k = n - 1;
    emxFree_creal_T(&b_fv);
    while (k + 1 <= wwc->size[0]) {
      e = wwc->data[k].re;
      b_fv_re = fv->data[k].re;
      denom_im = wwc->data[k].im;
      b_fv_im = fv->data[k].im;
      wwc_re = wwc->data[k].re;
      fv_im = fv->data[k].im;
      wwc_im = wwc->data[k].im;
      fv_re = fv->data[k].re;
      y->data[nd2].re = e * b_fv_re + denom_im * b_fv_im;
      y->data[nd2].im = wwc_re * fv_im - wwc_im * fv_re;
      e = wwc->data[k].re;
      b_fv_re = fv->data[k].re;
      denom_im = wwc->data[k].im;
      b_fv_im = fv->data[k].im;
      wwc_re = wwc->data[k].re;
      fv_im = fv->data[k].im;
      wwc_im = wwc->data[k].im;
      fv_re = fv->data[k].re;
      y->data[nd2].re = e * b_fv_re + denom_im * b_fv_im;
      y->data[nd2].im = wwc_re * fv_im - wwc_im * fv_re;
      e = y->data[nd2].re;
      denom_im = y->data[nd2].im;
      if (denom_im == 0.0) {
        y->data[nd2].re = e / (double)n;
        y->data[nd2].im = 0.0;
      } else if (e == 0.0) {
        y->data[nd2].re = 0.0;
        y->data[nd2].im = denom_im / (double)n;
      } else {
        y->data[nd2].re = e / (double)n;
        y->data[nd2].im = denom_im / (double)n;
      }

      nd2++;
      k++;
    }

    emxFree_creal_T(&fv);
    emxFree_creal_T(&wwc);
  }

  emxFree_real_T(&sintabinv);
  emxFree_real_T(&sintab);
  emxFree_real_T(&costab);
}

/*
 * Arguments    : int nRows
 *                boolean_T useRadix2
 *                emxArray_real_T *costab
 *                emxArray_real_T *sintab
 *                emxArray_real_T *sintabinv
 * Return Type  : void
 */
void generate_twiddle_tables(int nRows, boolean_T useRadix2, emxArray_real_T
  *costab, emxArray_real_T *sintab, emxArray_real_T *sintabinv)
{
  emxArray_real_T *costab1q;
  double e;
  int nRowsD4;
  int nd2;
  int k;
  int n2;
  emxInit_real_T(&costab1q, 2);
  e = 6.2831853071795862 / (double)nRows;
  nRowsD4 = nRows / 2 / 2;
  nd2 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = nRowsD4 + 1;
  emxEnsureCapacity_real_T(costab1q, nd2);
  costab1q->data[0] = 1.0;
  nd2 = nRowsD4 / 2;
  for (k = 1; k <= nd2; k++) {
    costab1q->data[k] = cos(e * (double)k);
  }

  for (k = nd2 + 1; k < nRowsD4; k++) {
    costab1q->data[k] = sin(e * (double)(nRowsD4 - k));
  }

  costab1q->data[nRowsD4] = 0.0;
  if (!useRadix2) {
    nRowsD4 = costab1q->size[1] - 1;
    n2 = (costab1q->size[1] - 1) << 1;
    nd2 = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = n2 + 1;
    emxEnsureCapacity_real_T(costab, nd2);
    nd2 = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = n2 + 1;
    emxEnsureCapacity_real_T(sintab, nd2);
    costab->data[0] = 1.0;
    sintab->data[0] = 0.0;
    nd2 = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = n2 + 1;
    emxEnsureCapacity_real_T(sintabinv, nd2);
    for (k = 1; k <= nRowsD4; k++) {
      sintabinv->data[k] = costab1q->data[nRowsD4 - k];
    }

    for (k = costab1q->size[1]; k <= n2; k++) {
      sintabinv->data[k] = costab1q->data[k - nRowsD4];
    }

    for (k = 1; k <= nRowsD4; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = -costab1q->data[nRowsD4 - k];
    }

    for (k = costab1q->size[1]; k <= n2; k++) {
      costab->data[k] = -costab1q->data[n2 - k];
      sintab->data[k] = -costab1q->data[k - nRowsD4];
    }
  } else {
    nRowsD4 = costab1q->size[1] - 1;
    n2 = (costab1q->size[1] - 1) << 1;
    nd2 = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = n2 + 1;
    emxEnsureCapacity_real_T(costab, nd2);
    nd2 = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = n2 + 1;
    emxEnsureCapacity_real_T(sintab, nd2);
    costab->data[0] = 1.0;
    sintab->data[0] = 0.0;
    for (k = 1; k <= nRowsD4; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = -costab1q->data[nRowsD4 - k];
    }

    for (k = costab1q->size[1]; k <= n2; k++) {
      costab->data[k] = -costab1q->data[n2 - k];
      sintab->data[k] = -costab1q->data[k - nRowsD4];
    }

    nd2 = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
    emxEnsureCapacity_real_T(sintabinv, nd2);
  }

  emxFree_real_T(&costab1q);
}

/*
 * Arguments    : int n1
 *                boolean_T useRadix2
 *                int *N2blue
 *                int *nRows
 * Return Type  : void
 */
void get_algo_sizes(int n1, boolean_T useRadix2, int *N2blue, int *nRows)
{
  int nn1m1;
  int pmax;
  int pmin;
  boolean_T exitg1;
  int p;
  int pow2p;
  *N2blue = 1;
  if (useRadix2) {
    *nRows = n1;
  } else {
    nn1m1 = (n1 + n1) - 1;
    pmax = 31;
    pmin = 0;
    exitg1 = false;
    while ((!exitg1) && (pmax - pmin > 1)) {
      p = (pmin + pmax) >> 1;
      pow2p = 1 << p;
      if (pow2p == nn1m1) {
        pmax = p;
        exitg1 = true;
      } else if (pow2p > nn1m1) {
        pmax = p;
      } else {
        pmin = p;
      }
    }

    *N2blue = 1 << pmax;
    *nRows = *N2blue;
  }
}

/*
 * Arguments    : const emxArray_creal_T *x
 *                int n1_unsigned
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void r2br_r2dit_trig(const emxArray_creal_T *x, int n1_unsigned, const
                     emxArray_real_T *costab, const emxArray_real_T *sintab,
                     emxArray_creal_T *y)
{
  double r;
  int i0;
  int loop_ub;
  b_r2br_r2dit_trig_impl(x, n1_unsigned, costab, sintab, y);
  if (y->size[0] > 1) {
    r = 1.0 / (double)y->size[0];
    i0 = y->size[0];
    emxEnsureCapacity_creal_T1(y, i0);
    loop_ub = y->size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      y->data[i0].re *= r;
      y->data[i0].im *= r;
    }
  }
}

/*
 * Arguments    : const emxArray_creal_T *x
 *                int unsigned_nRows
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void r2br_r2dit_trig_impl(const emxArray_creal_T *x, int unsigned_nRows, const
  emxArray_real_T *costab, const emxArray_real_T *sintab, emxArray_creal_T *y)
{
  int j;
  int nRowsD2;
  int nRowsD4;
  int iy;
  int iDelta;
  int ix;
  int ju;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  double twid_re;
  double twid_im;
  int ihi;
  j = x->size[0];
  if (!(j < unsigned_nRows)) {
    j = unsigned_nRows;
  }

  nRowsD2 = unsigned_nRows / 2;
  nRowsD4 = nRowsD2 / 2;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal_T1(y, iy);
  if (unsigned_nRows > x->size[0]) {
    iDelta = y->size[0];
    iy = y->size[0];
    y->size[0] = iDelta;
    emxEnsureCapacity_creal_T1(y, iy);
    for (iy = 0; iy < iDelta; iy++) {
      y->data[iy].re = 0.0;
      y->data[iy].im = 0.0;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 1; i < j; i++) {
    y->data[iy] = x->data[ix];
    iDelta = unsigned_nRows;
    tst = true;
    while (tst) {
      iDelta >>= 1;
      ju ^= iDelta;
      tst = ((ju & iDelta) == 0);
    }

    iy = ju;
    ix++;
  }

  y->data[iy] = x->data[ix];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= unsigned_nRows - 2; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
  }

  iDelta = 2;
  iy = 4;
  ix = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ix; i += iy) {
      temp_re = y->data[i + iDelta].re;
      temp_im = y->data[i + iDelta].im;
      y->data[i + iDelta].re = y->data[i].re - temp_re;
      y->data[i + iDelta].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }

    ju = 1;
    for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      i = ju;
      ihi = ju + ix;
      while (i < ihi) {
        temp_re = twid_re * y->data[i + iDelta].re - twid_im * y->data[i +
          iDelta].im;
        temp_im = twid_re * y->data[i + iDelta].im + twid_im * y->data[i +
          iDelta].re;
        y->data[i + iDelta].re = y->data[i].re - temp_re;
        y->data[i + iDelta].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iy;
      }

      ju++;
    }

    nRowsD4 /= 2;
    iDelta = iy;
    iy += iy;
    ix -= iDelta;
  }
}

/*
 * File trailer for fft1.c
 *
 * [EOF]
 */
