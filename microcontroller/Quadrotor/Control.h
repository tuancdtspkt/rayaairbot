#if !defined( CONTROL_H )
#define CONTROL_H

#include "Quadrotor.h"
#include "Pid.h"

extern int16_t u[4];

extern struct PID pid[2];

extern int8_t emergenciaSTOP;
extern int16_t omega; 

void Control(void);
void zeroIntegralError();

#endif
