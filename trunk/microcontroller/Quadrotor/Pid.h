#if !defined( PID_H )
#define PID_H

#include <avr/io.h>

struct PID {
  int16_t P, I, D;
  int16_t lastPosition;
  int16_t integratedError;
  int16_t windUpGuard;
  int16_t e;
  int16_t r;
};

int16_t UpdatePID(struct PID *pid, int16_t referencia, int16_t posicion_actual);


#endif
