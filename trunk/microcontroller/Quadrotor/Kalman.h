#if !defined( KALMAN_H )
#define KALMAN_H

void state_update(float q_m	/* Pitch gyro measurement */);
float kalman_update(float angle_m);//float ax_m,	/* X acceleration */float az_m	/* Z acceleration */);

#endif
