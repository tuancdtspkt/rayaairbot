//#include <stdio.h>
#include "Pid.h"

int16_t UpdatePID(struct PID *pid, int16_t referencia, int16_t posicion_actual)
{
    int16_t e = referencia - posicion_actual;

    pid->integratedError += e;

    // Antienrrollamiento
    if(pid->integratedError > pid->windUpGuard)
        pid->integratedError = pid->windUpGuard;
    else if(pid->integratedError < -pid->windUpGuard)
        pid->integratedError = -pid->windUpGuard;

    int16_t result = (pid->P*e)/10 + (pid->integratedError/100)*pid->I + (pid->D*(posicion_actual - pid->lastPosition))/10;

    pid->lastPosition = posicion_actual;

pid->r=result;
pid->e=e;

    return result;
}
