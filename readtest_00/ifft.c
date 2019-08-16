/*
 * File: ifft.c
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "modwt1.h"
#include "ifft.h"
#include "modwt1_emxutil.h"
#include "fft1.h"

/* Function Definitions */

/*
 * Arguments    : const emxArray_creal_T *x
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void ifft(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  emxArray_creal_T *r2;
  int b_x[1];
  emxArray_creal_T c_x;
  int i1;
  int loop_ub;
  emxInit_creal_T1(&r2, 1);
  b_x[0] = x->size[1];
  c_x = *x;
  c_x.size = (int *)&b_x;
  c_x.numDimensions = 1;
  c_fft(&c_x, x->size[1], r2);
  i1 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity_creal_T(y, i1);
  loop_ub = x->size[1];
  for (i1 = 0; i1 < loop_ub; i1++) {
    y->data[i1] = r2->data[i1];
  }

  emxFree_creal_T(&r2);
}

/*
 * File trailer for ifft.c
 *
 * [EOF]
 */
