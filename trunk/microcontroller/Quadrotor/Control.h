#if !defined( CONTROL_H )
#define CONTROL_H

#include "Quadrotor.h"

extern int16_t u[4];

extern int8_t P[2];
extern int8_t I[2];
extern int8_t D[2];

extern int16_t I_Max[2];
extern int16_t I_Min[2];

extern int8_t emergenciaSTOP;
extern int16_t omega; 

void Control(void);

#endif