#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdint.h>

typedef struct
{
    uint16_t  col;
    uint16_t  row;
    float     **mat;
}matrix_t;


#endif //__MATRIX_H

