#ifndef CUMO_SFLOAT_KERNEL_H
#define CUMO_SFLOAT_KERNEL_H

typedef float dtype;
typedef float rtype;

#include "float_macro_kernel.h"

#define m_nearly_eq(x,y) (fabs(x-y)<=(fabs(x)+fabs(y))*FLT_EPSILON*2)

#define DATA_MIN -FLT_MAX
#define DATA_MAX FLT_MAX

#endif // CUMO_SFLOAT_KERNEL_H
