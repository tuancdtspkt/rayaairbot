//#include <stdio.h>
#include "Pid.h"

int16_t UpdatePID(struct *pid, int16_t referencia, int16_t posicion_actual)
{
    int16_t e = referencia - posicion_actual;

    pid->integratedError += e;

    // Antienrrollamiento
    if(pid->integratedError > pid->windUpGuard)
        pid->integratedError = pid->windUpGuard;
    else if(pid->integratedError < -pid->windUpGuard)
        pid->integratedError = -pid->windUpGuard;

    int16_t result = (pid->P*e)/10 + (pid->integratedError*pid->I)/10 + (pid->D*(posicion_actual - pid->lastPosition))/10;

    pid->lastPosition = posicion_actual;

    return result;
}
