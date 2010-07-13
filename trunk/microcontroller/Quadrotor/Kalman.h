#if !defined( KALMAN_H )
#define KALMAN_H

void KalmanStateUpdate(float q_m	/* Pitch gyro measurement */);
float KalmanUpdate(float angle_m);//float ax_m,	/* X acceleration */float az_m	/* Z acceleration */);

#endif
