#pragma once
#include <cmath>

struct Vector4
{
    float x, y, z, w;
    Vector4(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Vector4() : Vector4(0, 0, 0, 0) {}

    float Magnitude()
    {
        return sqrt(x*x+y*y+z*z+w*w);
    }
    Vector4 Normalized()
    {
        float mag = this->Magnitude();
        if(mag == 0)
            return Vector4();
        return *this/mag;
    }

    Vector4 operator +(Vector4 other)
    {
        return Vector4(this->x+other.x, this->y+other.y, this->z+other.z, this->w+other.w);
    }
    Vector4 operator -(Vector4 other)
    {
        return Vector4(this->x-other.x, this->y-other.y, this->z-other.z, this->w-other.w);
    }
    Vector4 operator /(float s)
    {
        return Vector4(this->x/s, this->y/s, this->z/s, this->w/s);
    }

    void log()
    {
        printf("{%f, %f, %f, %f}\n", this->x, this->y, this->z, this->w);
    }

    Vector4 cross(Vector4 v2)
    {
        return Vector4(
          this->y * v2.z - this->z * v2.y,
          this->z * v2.x - this->x * v2.z,
          this->x * v2.y - this->y * v2.x,
          0
        );
    }
};