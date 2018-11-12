#ifndef __DISPLAY
#define	__DISPLAY
//#include <rtthread.h>
#include "sys_def.h"

/*
0:    barometric pressure (mbar)
1:    temprature(C) x 100
2:    PH x 100
3:    oxidation/reduction potential(mv x 10)
4:    turbidity (NTU x 10)

9:    Electrical conductivity (uS /cm)(upper 16bit)
10:   Electrical conductivity (uS /cm)(lower 16bit)
11:   Electrical resistivity (O.cm) upper 16 bits
12:   Electrical resistivity (O.cm) lower 16 bits
13:   Sanility(PSU x 100)
14:   Total dissolved solids (mg/L) upper 16 bits
15:   Total dissolved solids (mg/L) lower 16 bits
16:   Specific seawater gravity (dt x 10)
17:   Desolved Oxigen (mg/L x 100)
18:   Desolved Oxigen (% air saturation x 10)
19:   probe depth (cm)

32:   Ammonia concentration(mg/L) upper 16 bits
33:   Ammonia concentration(mg/L) lower 16 bits
*/

typedef struct
{	
    uint32_t 		barometric;
    uint32_t 		temprature;
    uint32_t 		oxidation;
    uint32_t 		ph;
    uint32_t 		turbidity;
    uint32_t 		condutivity;
    uint32_t 		resistivity;
    uint32_t 		sanility;
    uint32_t 		tds;
    uint32_t 		ssg;
    uint32_t 		desolved_oxigen;
    uint32_t 		desolved_oxigen_air;
    uint32_t 		probe_depth;
    uint32_t 		ammonia_concentration;
    uint32_t 		nitrate_concentration;
}sensor_multi_st;

typedef struct
{	
    char 		barometric[16];
    char 		temprature[16];
    char 		oxidation[16];
    char 		ph[16];
    char 		turbidity[16];
    char 		condutivity[16];
    char 		resistivity[16];
    char 		sanility[16];
    char 		tds[16];
    char 		ssg[16];
    char 		desolved_oxigen[16];
    char 		desolved_oxigen_air[16];
    char 		probe_depth[16];
    char 		ammonia_concentration[16];
    char 		nitrate_concentration[16];    
}sensor_multi_str_st;

typedef struct
{	
    uint32_t 		desolved_o2;
    uint32_t 		temprature;
}sensor_do2_st;

typedef struct
{	
    uint32_t 		ph_val;
    uint32_t 		temprature;
}sensor_ph_st;


typedef struct
{	
    sensor_multi_st     multi;
    sensor_multi_str_st multi_str;
    sensor_ph_st        ph;
    sensor_do2_st       do2;
//    gen_st              gen;
}disp_st;

typedef struct
{	
    int8_t 		barometric[24];
    int8_t 		temprature[24];
    int8_t 		ph[24];
    int8_t 		oxidation[24];    
    int8_t 		turbidity[24];
    int8_t 		condutivity[24];
    int8_t 		resistivity[24];
    int8_t 		sanility[24];
    int8_t 		tds[24];
    int8_t 		ssg[24];
    int8_t 		desolved_oxigen[24];
    int8_t 		desolved_oxigen_air[24];
    int8_t 		probe_depth[24];
    int8_t 		ammonia_concentration[24];
    int8_t 		nitrate_concentration[24];
}sensor_namesting_st;

void disp_init(void);
void disp_fresh(void);

#endif //	__DISPLAY
