/*
 * Axis.h
 *
 *      Author: Juande Estrella
 */

#ifndef MPU6050_AXIS_H_
#define MPU6050_AXIS_H_

struct Axis {
public:
    float x;
    float y;
    float z;

    Axis();
};


#endif /* MPU6050_AXIS_H_ */
