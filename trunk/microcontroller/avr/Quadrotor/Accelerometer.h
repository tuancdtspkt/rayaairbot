#if !defined( ACCELEROMETER_H )
#define ACCELEROMETER_H

extern int16_t accelerometer[3];
extern int16_t angle[2];

void InitAccelerometer(void);
void GetAccelerometer(void);

// Multiply by this factor to convert accelerometer[] in g units.
// 2.56/1023/0.3
#define CONVERT_TO_G  0.00834147931f

#endif
