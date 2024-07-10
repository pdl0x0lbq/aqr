//
// Created by sw on 24-6-4.
//

#ifndef AQR_CONTROL_BMATH_H
#define AQR_CONTROL_BMATH_H
struct Vector {
    double x;
    double y;
};
double dotProduct(const Vector& a, const Vector& b);
double norm(const Vector& v);
double angleBetweenVectors(const Vector& v1, const Vector& eAa);
double adjustAngle(double angle);
#endif //AQR_CONTROL_BMATH_H
