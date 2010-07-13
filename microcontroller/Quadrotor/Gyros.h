#if !defined( GYROS_H )
#define GYROS_H

extern int16_t gyro[2];

void InitGyros(void);
void GetGyros(void);

// Multiply by this factor to convert gyro[] in rad/s units.
#define CONVERT_TO_RAD_S 0.0131030725 

// Multiply by this factor to convert gyro[] in degree/s units.
#define CONVERT_TO_DEGREE_S 0.750750753


#endif
