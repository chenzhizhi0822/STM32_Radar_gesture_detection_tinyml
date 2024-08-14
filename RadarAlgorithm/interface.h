#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "./infineon_vitalsign_float/transfC.h"
#include "./infineon_vitalsign_interpeak/interpeaks.h"

//算法的函数指针放到这里
typedef void (*transfC_ptr)(const unsigned short *, float *, float *);
// typedef void （*interpeaks_ptr)(const unsigned short *, float *heart_est_inter, float *breath_est_inter);

#endif // __INTERFACE_H__