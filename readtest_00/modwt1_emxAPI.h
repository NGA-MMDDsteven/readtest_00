/*
 * File: modwt1_emxAPI.h
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

#ifndef MODWT1_EMXAPI_H
#define MODWT1_EMXAPI_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "modwt1_types.h"

/* Function Declarations */
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for modwt1_emxAPI.h
 *
 * [EOF]
 */
