#include "interface.h"

// 算法所有的c文件
// #include "./infineon_vitalsign_float/transfC.c"
// #include "./infineon_vitalsign_float/FFTImplementationCallback.c"
// #include "./infineon_vitalsign_float/fftshift.c"
// #include "./infineon_vitalsign_float/filter.c"
// #include "./infineon_vitalsign_float/rt_nonfinite.c"
// #include "./infineon_vitalsign_float/rtGetInf.c"
// #include "./infineon_vitalsign_float/rtGetNaN.c"
// #include "./infineon_vitalsign_float/transfC_data.c"
// #include "./infineon_vitalsign_float/transfC_initialize.c"
// #include "./infineon_vitalsign_float/transfC_terminate.c"

#include "./infineon_vitalsign_interpeak/eml_setop.c"
#include "./infineon_vitalsign_interpeak/FFTImplementationCallback.c"
#include "./infineon_vitalsign_interpeak/filter.c"
#include "./infineon_vitalsign_interpeak/findpeaks.c"
#include "./infineon_vitalsign_interpeak/interpeaks.c"
#include "./infineon_vitalsign_interpeak/interpeaks_data.c"
#include "./infineon_vitalsign_interpeak/interpeaks_initialize.c"
#include "./infineon_vitalsign_interpeak/interpeaks_terminate.c"
#include "./infineon_vitalsign_interpeak/rt_nonfinite.c"
#include "./infineon_vitalsign_interpeak/rtGetInf.c"
#include "./infineon_vitalsign_interpeak/rtGetNaN.c"

// interface_Algo 是一个单片机软件使用的算法接口，需要将实现的算法函数指针更新到这里
// transfC_ptr interface_Algo = transfC;
transfC_ptr interface_Algo = interpeaks;
