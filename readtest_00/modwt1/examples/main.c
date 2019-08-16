/*
 * File: main.c
 *
 * MATLAB Coder version            : 4.0
 * C/C++ source code generated on  : 07-Jun-2019 09:05:14
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
/* Include Files */
#include "rt_nonfinite.h"
#include "modwt1.h"
#include "main.h"
#include "modwt1_terminate.h"
#include "modwt1_emxAPI.h"
#include "modwt1_initialize.h"

/* Function Declarations */
static emxArray_real_T *argInit_1xUnbounded_real_T(void);
static double argInit_real_T(void);
static void main_modwt1(void);

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : emxArray_real_T *
 */
static emxArray_real_T *argInit_1xUnbounded_real_T(void)
{
  emxArray_real_T *result;
  static int iv0[2] = { 1, 2 };

  int idx1;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result = emxCreateND_real_T(2, iv0);

  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result->data[result->size[0] * idx1] = argInit_real_T();
  }

  return result;
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_modwt1(void)
{
  emxArray_real_T *w;
  emxArray_real_T *x;
  emxInitArray_real_T(&w, 2);

  /* Initialize function 'modwt1' input arguments. */
  /* Initialize function input argument 'x'. */
  x = argInit_1xUnbounded_real_T();

  /* Call the entry-point 'modwt1'. */
  modwt1(x, argInit_real_T(), w);
  emxDestroyArray_real_T(w);
  emxDestroyArray_real_T(x);
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* Initialize the application.
     You do not need to do this more than one time. */
  modwt1_initialize();

  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_modwt1();

  /* Terminate the application.
     You do not need to do this more than one time. */
  modwt1_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
