#if !defined( PID_H )
#define PID_H

#define WIND_UP_GUARD 1000

struct PID {
  int16_t P, I, D;
  int16_t lastPosition;
  int16_t integratedError;
};

void UpdatePID(struct *pid, int16_t referencia, int16_t posicion_actual);


#endif
