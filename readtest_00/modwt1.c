/*
 * File: modwt1.c
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

/* Include Files */
#include <math.h>
#include "rt_nonfinite.h"
#include "modwt1.h"
#include "modwt1_emxutil.h"
#include "ifft.h"
#include "fft.h"

/* Function Declarations */
static int div_s32(int numerator, int denominator);

/* Function Definitions */

/*
 * Arguments    : int numerator
 *                int denominator
 * Return Type  : int
 */
static int div_s32(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      absNumerator = ~(unsigned int)numerator + 1U;
    } else {
      absNumerator = (unsigned int)numerator;
    }

    if (denominator < 0) {
      absDenominator = ~(unsigned int)denominator + 1U;
    } else {
      absDenominator = (unsigned int)denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    absNumerator /= absDenominator;
    if (quotientNeedsNegation) {
      quotient = -(int)absNumerator;
    } else {
      quotient = (int)absNumerator;
    }
  }

  return quotient;
}

/*
 * Arguments    : const emxArray_real_T *x
 *                double lev
 *                emxArray_real_T *w
 * Return Type  : void
 */
void modwt1(const emxArray_real_T *x, double lev, emxArray_real_T *w)
{
  int n;
  int offset;
  int eint;
  int ncopies;
  int Nrep;
  emxArray_real_T *xx;
  int k;
  emxArray_creal_T *G;
  emxArray_creal_T *H;
  emxArray_creal_T *Vhat;
  static const double Lo[8] = { 0.022785172947974931, -0.0089123507208401943,
    -0.070158812089422817, 0.21061726710176826, 0.56832912170437477,
    0.35186953432761287, -0.020955482562526946, -0.053574450708941054 };

  static const double Hi[8] = { -0.053574450708941054, 0.020955482562526946,
    0.35186953432761287, -0.56832912170437477, 0.21061726710176826,
    0.070158812089422817, -0.0089123507208401943, -0.022785172947974931 };

  int jj;
  emxArray_creal_T *b_Vhat;
  emxArray_creal_T *What;
  double G_re;
  double G_im;
  double Vhat_re;
  double Vhat_im;
  n = x->size[1];
  frexp(x->size[1], &offset);
  frexp(x->size[1], &eint);
  if (x->size[1] < 8) {
    ncopies = 8 - x->size[1];
    Nrep = (9 - x->size[1]) * x->size[1];
  } else {
    ncopies = 0;
    Nrep = x->size[1];
  }

  emxInit_real_T(&xx, 2);
  eint = xx->size[0] * xx->size[1];
  xx->size[0] = 1;
  xx->size[1] = Nrep;
  emxEnsureCapacity_real_T(xx, eint);
  offset = x->size[1];
  for (eint = 0; eint < offset; eint++) {
    xx->data[eint] = x->data[x->size[0] * eint];
  }

  if (ncopies > 0) {
    for (k = 1; k <= ncopies; k++) {
      offset = k * n;
      for (eint = 0; eint < n; eint++) {
        xx->data[offset + eint] = xx->data[eint];
      }
    }
  }

  eint = w->size[0] * w->size[1];
  w->size[0] = (int)lev + 1;
  w->size[1] = x->size[1];
  emxEnsureCapacity_real_T(w, eint);
  offset = ((int)lev + 1) * x->size[1];
  for (eint = 0; eint < offset; eint++) {
    w->data[eint] = 0.0;
  }

  emxInit_creal_T(&G, 2);
  emxInit_creal_T(&H, 2);
  emxInit_creal_T(&Vhat, 2);
  fft(Lo, Nrep, G);
  fft(Hi, Nrep, H);
  b_fft(xx, Vhat);
  jj = 0;
  emxInit_creal_T(&b_Vhat, 2);
  emxInit_creal_T(&What, 2);
  while (jj + 1 <= (int)lev) {
    ncopies = Vhat->size[1];
    Nrep = 1 << jj;
    eint = b_Vhat->size[0] * b_Vhat->size[1];
    b_Vhat->size[0] = Vhat->size[0];
    b_Vhat->size[1] = Vhat->size[1];
    emxEnsureCapacity_creal_T(b_Vhat, eint);
    eint = What->size[0] * What->size[1];
    What->size[0] = Vhat->size[0];
    What->size[1] = Vhat->size[1];
    emxEnsureCapacity_creal_T(What, eint);
    for (k = 0; k < ncopies; k++) {
      offset = Nrep * k;
      offset -= div_s32(offset, Vhat->size[1]) * ncopies;
      G_re = G->data[offset].re;
      G_im = G->data[offset].im;
      Vhat_re = Vhat->data[k].re;
      Vhat_im = Vhat->data[k].im;
      b_Vhat->data[k].re = G_re * Vhat_re - G_im * Vhat_im;
      b_Vhat->data[k].im = G_re * Vhat_im + G_im * Vhat_re;
      G_re = H->data[offset].re;
      G_im = H->data[offset].im;
      Vhat_re = Vhat->data[k].re;
      Vhat_im = Vhat->data[k].im;
      What->data[k].re = G_re * Vhat_re - G_im * Vhat_im;
      What->data[k].im = G_re * Vhat_im + G_im * Vhat_re;
    }

    eint = Vhat->size[0] * Vhat->size[1];
    Vhat->size[0] = 1;
    Vhat->size[1] = b_Vhat->size[1];
    emxEnsureCapacity_creal_T(Vhat, eint);
    offset = b_Vhat->size[0] * b_Vhat->size[1];
    for (eint = 0; eint < offset; eint++) {
      Vhat->data[eint] = b_Vhat->data[eint];
    }

    ifft(What, b_Vhat);
    eint = xx->size[0] * xx->size[1];
    xx->size[0] = 1;
    xx->size[1] = b_Vhat->size[1];
    emxEnsureCapacity_real_T(xx, eint);
    offset = b_Vhat->size[0] * b_Vhat->size[1];
    for (eint = 0; eint < offset; eint++) {
      xx->data[eint] = b_Vhat->data[eint].re;
    }

    for (eint = 0; eint < n; eint++) {
      w->data[jj + w->size[0] * eint] = xx->data[eint];
    }

    jj++;
  }

  emxFree_creal_T(&What);
  emxFree_creal_T(&H);
  emxFree_creal_T(&G);
  ifft(Vhat, b_Vhat);
  eint = xx->size[0] * xx->size[1];
  xx->size[0] = 1;
  xx->size[1] = b_Vhat->size[1];
  emxEnsureCapacity_real_T(xx, eint);
  offset = b_Vhat->size[0] * b_Vhat->size[1];
  emxFree_creal_T(&Vhat);
  for (eint = 0; eint < offset; eint++) {
    xx->data[eint] = b_Vhat->data[eint].re;
  }

  emxFree_creal_T(&b_Vhat);
  offset = x->size[1] - 1;
  for (eint = 0; eint <= offset; eint++) {
    w->data[(int)lev + w->size[0] * eint] = xx->data[eint];
  }

  emxFree_real_T(&xx);
}

/*
 * File trailer for modwt1.c
 *
 * [EOF]
 */
