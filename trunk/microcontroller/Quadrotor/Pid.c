//#include <stdio.h>
#include "Pid.h"

int16_t UpdatePID(struct PID *pid, int16_t referencia, int16_t posicion_actual)
{
//printf("l%d\n", posicion_actual);
    pid->lastPosition = pid->e;
    pid->e = referencia - posicion_actual;

    pid->integratedError += pid->e;

    // Antienrrollamiento
    if(pid->integratedError > pid->windUpGuard)
        pid->integratedError = pid->windUpGuard;
    else if(pid->integratedError < -pid->windUpGuard)
        pid->integratedError = -pid->windUpGuard;

    pid->r = (pid->P*pid->e)/100 + (pid->integratedError/100)*pid->I + (pid->D*(pid->e - pid->lastPosition))/10;


    return pid->r;
}
