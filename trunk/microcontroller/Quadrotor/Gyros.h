#if !defined( GYROS_H )
#define GYROS_H

extern int16_t gyro[2];
extern int16_t gyro_angle[2];

void InitGyros(void);
void GetGyros(void); 

// Multiply by this factor to convert gyro[] in rad/s units.
// (2.56/1023)/0.00333 * pi/180
#define CONVERT_TO_RAD_S 0.0131158809f


// Multiply by this factor to convert gyro[] in degree/s units.
// (2.56/1023)/0.00333
#define CONVERT_TO_DEGREE_S 0.751484622f


#endif
