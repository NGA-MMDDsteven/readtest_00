/*
 * File: _coder_modwt1_api.h
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

#ifndef _CODER_MODWT1_API_H
#define _CODER_MODWT1_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_modwt1_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void modwt1(emxArray_real_T *x, real_T lev, emxArray_real_T *w);
extern void modwt1_api(const mxArray * const prhs[2], int32_T nlhs, const
  mxArray *plhs[1]);
extern void modwt1_atexit(void);
extern void modwt1_initialize(void);
extern void modwt1_terminate(void);
extern void modwt1_xil_terminate(void);

#endif

/*
 * File trailer for _coder_modwt1_api.h
 *
 * [EOF]
 */
