/*
 * File: modwt1_emxutil.h
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

#ifndef MODWT1_EMXUTIL_H
#define MODWT1_EMXUTIL_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "modwt1_types.h"

/* Function Declarations */
extern void emxEnsureCapacity_creal_T(emxArray_creal_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_creal_T1(emxArray_creal_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
extern void emxFree_creal_T(emxArray_creal_T **pEmxArray);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_creal_T(emxArray_creal_T **pEmxArray, int numDimensions);
extern void emxInit_creal_T1(emxArray_creal_T **pEmxArray, int numDimensions);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for modwt1_emxutil.h
 *
 * [EOF]
 */
