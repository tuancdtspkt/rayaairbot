//#include <stdio.h>
#include "Pid.h"

//WIND_UP_GUARD
struct PID {
  int P, I, D;
  int lastPosition;
  int integratedError;
};

int16_t UpdatePID(struct *pid, int16_t referencia, int16_t posicion_actual)
{
    int16_t e = referencia - posicion_actual;

    pid->integratedError += e;

    // Antienrrollamiento
    if(pid->integratedError > WIND_UP_GUARD)
        pid->integratedError = WIND_UP_GUARD;
    else if(pid->integratedError < -WIND_UP_GUARD)
        pid->integratedError = -WIND_UP_GUARD;

    int16_t result = (pid->P*e)/10 + (pid->integratedError*pid->I)/10 + (pid->D*(posicion_actual - pid->lastPosition))/10;

    pid->lastPosition = posicion_actual;

    return result;
}
