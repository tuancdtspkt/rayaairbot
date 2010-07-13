#if !defined( GYROS_H )
#define GYROS_H

extern int16_t gyro[2];

void InitGyros(void);
void GetGyros(void);

// Multiply by this factor tu convert gyro[] in rad/s units.
#define CONVERT_TO_RAD_S 0.0131030725 


#endif
