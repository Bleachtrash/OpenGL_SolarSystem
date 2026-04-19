#pragma once
#include <cmath>

struct Vector3
{
    float x, y, z;
    Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3() : Vector3(0, 0, 0){}
    float Magnitude()
    {
        return sqrt(x*x+y*y+z*z);
    }
    Vector3 Normalized()
    {
        float mag = this->Magnitude();
        if(mag == 0)
            return Vector3();
        return *this/mag;
    }

    // Operators
    Vector3 operator +(Vector3 other)
    {
        return Vector3(this->x+other.x, this->y+other.y, this->z+other.z);
    }
    Vector3 operator -(Vector3 other)
    {
        return Vector3(this->x-other.x, this->y-other.y, this->z-other.z);
    }

    Vector3 operator *(float s)
    {
        return Vector3(this->x*s, this->y*s, this->z*s);
    }
    Vector3 operator /(float s)
    {
        return Vector3(this->x/s, this->y/s, this->z/s);
    }

    Vector3 cross(Vector3 v2)
    {
        return Vector3(
            this->y * v2.z - this->z * v2.y,
            this->z * v2.x - this->x * v2.z,
            this->x * v2.y - this->y * v2.x
        );
    }
    void log()
    {
        printf("{%f, %f, %f}\n", this->x, this->y, this->z);
    }
};