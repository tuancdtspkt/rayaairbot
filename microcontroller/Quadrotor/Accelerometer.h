#if !defined( ACCELEROMETER_H )
#define ACCELEROMETER_H

extern int16_t accelerometer[3];
extern int16_t angle[2];

void InitAccelerometer(void);
void GetAccelerometer(void);

// Multiply by this factor tu convert accelerometer[] in g units.
#define CONVERT_TO_G 120

#endif
