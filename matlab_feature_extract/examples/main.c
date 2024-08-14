/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Aug-2024 17:37:12
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
#include "main.h"
#include "matlab_feature_extract.h"
#include "matlab_feature_extract_terminate.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void argInit_64x32x3_uint16_T(unsigned short result[6144]);

static unsigned short argInit_uint16_T(void);

static unsigned int argInit_uint32_T(void);

/* Function Definitions */
/*
 * Arguments    : unsigned short result[6144]
 * Return Type  : void
 */
static void argInit_64x32x3_uint16_T(unsigned short result[6144])
{
  int idx0;
  int idx1;
  int idx2;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 64; idx0++) {
    for (idx1 = 0; idx1 < 32; idx1++) {
      for (idx2 = 0; idx2 < 3; idx2++) {
        /* Set the value of the array element.
Change this value to the value that the application requires. */
        result[(idx0 + (idx1 << 6)) + (idx2 << 11)] = argInit_uint16_T();
      }
    }
  }
}

/*
 * Arguments    : void
 * Return Type  : unsigned short
 */
static unsigned short argInit_uint16_T(void)
{
  return 0U;
}

/*
 * Arguments    : void
 * Return Type  : unsigned int
 */
static unsigned int argInit_uint32_T(void)
{
  return 0U;
}

/*
 * Arguments    : int argc
 *                char **argv
 * Return Type  : int
 */
int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  /* The initialize function is being called automatically from your entry-point
   * function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
You can call entry-point functions multiple times. */
  main_matlab_feature_extract();
  /* Terminate the application.
You do not need to do this more than one time. */
  matlab_feature_extract_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_matlab_feature_extract(void)
{
  float featuresMatrix[6];
  unsigned short uv[6144];
  /* Initialize function 'matlab_feature_extract' input arguments. */
  /* Initialize function input argument 'Data'. */
  /* Call the entry-point 'matlab_feature_extract'. */
  argInit_64x32x3_uint16_T(uv);
  matlab_feature_extract(uv, argInit_uint32_T(), featuresMatrix);
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
