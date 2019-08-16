/*
 * File: fft.h
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

#ifndef FFT_H
#define FFT_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "modwt1_types.h"

/* Function Declarations */
extern void b_fft(const emxArray_real_T *x, emxArray_creal_T *y);
extern void fft(const double x[8], double varargin_1, emxArray_creal_T *y);

#endif

/*
 * File trailer for fft.h
 *
 * [EOF]
 */
