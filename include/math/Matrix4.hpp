#pragma once
#include <iostream>
#define te this->elements

#include <cmath>

#include <math/Vector3.hpp>
#include <math/Vector4.hpp>

#define PI 3.14159265358979
struct Matrix4
{
    float *elements = new float[16];

    ~Matrix4()
    {
        // delete elements;
    }
    Matrix4()
    {
        this->makeIdentity();
    }
    Matrix4 clone()
    {
        Matrix4 res;
        for(int i = 0; i < 16; i++)
        {
            res.elements[i] = this->elements[i];
        }
        return res;
    }
    // Get/set elements
    float getElement(int row, int col)
    {
        return elements[row*4+col];
    }
    void setElement(int row, int col, float val)
    {
        elements[row*4+col] = val;
    }

    Matrix4 makeIdentity()
    {
        for(int i = 0; i < 16; i++)
        {
            this->elements[i] = 0;
            if(i%5==0)
                this->elements[i]=1;
        }
        return *this;
    }
    // Rotations
    Matrix4 makeRotationX(float degrees)
    {
        float rads = degrees*PI/180;
        
        float c = cos(rads);
        float s = sin(rads);
        te[0] = 1; te[1] = 0; te[2] = 0; te[3] = 0;
        te[4] = 0; te[5] = c; te[6] = -s; te[7] = 0;
        te[8] = 0; te[9] = s; te[10] = c; te[11] = 0;
        te[12] = 0; te[13] = 0; te[14] = 0; te[15] = 1;

        return *this;
    }
    Matrix4 makeRotationY(float degrees)
    {
        float rads = degrees*PI/180;
        this->makeIdentity();

        float c = cos(rads);
        float s = sin(rads);

        te[0] = c; te[1] = 0; te[2] = s; te[3] = 0;
        te[4] = 0; te[5] = 1; te[6] = 0; te[7] = 0;
        te[8] = -s; te[9] = 0; te[10] = c; te[11] = 0;
        te[12] = 0; te[13] = 0; te[14] = 0; te[15] = 1;
        
        return *this;
    }
    Matrix4 makeRotationZ(float degrees)
    {
        float rads = degrees*PI/180;
        this->makeIdentity();

        float c = cos(rads);
        float s = sin(rads);

        te[0] = c; te[1] = -s; te[2] = 0; te[3] = 0;
        te[4] = s; te[5] = c; te[6] = 0; te[7] = 0;
        te[8] = 0; te[9] = 0; te[10] = 1; te[11] = 0;
        te[12] = 0; te[13] = 0; te[14] = 0; te[15] = 1;
        
        return *this;
    }
    // misc transformations
    Matrix4 makeScale(float x, float y, float z)
    {
        this->elements[0] = x;
        this->elements[5] = y;
        this->elements[10] = z;

        return *this;
    }
    Matrix4 makeTranslation(float x, float y, float z)
    {
        this->makeIdentity();
        this->elements[3] = x;
        this->elements[7] = y;
        this->elements[11] = z;

        return *this;
    }
    Matrix4 makeTranslation(Vector3 a)
    {
        this->makeIdentity();
        this->elements[3] = a.x;
        this->elements[7] = a.y;
        this->elements[11] = a.z;

        return *this;
    }
    Matrix4 makeTranslation(Vector4 a)
    {
        this->makeIdentity();
        this->elements[3] = a.x;
        this->elements[7] = a.y;
        this->elements[11] = a.z;

        return *this;
    }
    Matrix4 transpose()
    {
        float tmp;
        tmp = te[1]; te[1] = te[4]; te[4] = tmp;
        tmp = te[2]; te[2] = te[8]; te[8] = tmp;
        tmp = te[6]; te[6] = te[9]; te[9] = tmp;

        tmp = te[3]; te[3] = te[12]; te[12] = tmp;
        tmp = te[7]; te[7] = te[13]; te[13] = tmp;
        tmp = te[11]; te[11] = te[14]; te[14] = tmp;
        
        return *this;
    }
    Matrix4 inverse()
    {
        float   n11 = te[0], n21 = te[1], n31 = te[2], n41 = te[3],
                n12 = te[4], n22 = te[5], n32 = te[6], n42 = te[7],
                n13 = te[8], n23 = te[9], n33 = te[10], n43 = te[11],
                n14 = te[12], n24 = te[13], n34 = te[14], n44 = te[15];

        float   t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44,
                t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44,
                t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44,
                t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

        float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
        if(det == 0)
        {
            return Matrix4();
        }

        float detInv = 1 / det;

        te[0] = t11 * detInv;
        te[1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * detInv;
        te[2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * detInv;
        te[3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * detInv;

        te[4] = t12 * detInv;
        te[5] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * detInv;
        te[6] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * detInv;
        te[7] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * detInv;

        te[8] = t13 * detInv;
        te[9] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * detInv;
        te[10] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * detInv;
        te[11] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * detInv;

        te[12] = t14 * detInv;
        te[13] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * detInv;
        te[14] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * detInv;
        te[15] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * detInv;
        return *this;
    }
    // Perspective stuff
    void makePerspective(float fovy, float aspect, float near, float far)
    {
        this->makeIdentity();

        float fovyRads = fovy*PI/180;

        // compute t(top) and r(right)
        float t = tan(fovyRads/2)*near;
        float r = t*aspect;

        te[0] = near/r;
        te[5] = near/t;
        te[10] = -((far+near)/(far-near));
        te[11] = -((2*far*near)/(far-near));
        te[14] = -1;
        te[15] = 0;
    }
    void makeOrthographic(float left, float right, float top, float bottom, float near, float far)
    {
        this->makeIdentity();

        te[0]  = 2/(right-left);
        te[3]  = -(right+left)/(right-left);
        te[5]  = 2/(top-bottom);
        te[7]  = -(top+bottom)/(top-bottom);
        te[10] = -2/(far-near);
        te[11] = -(far+near)/(far-near);
    }

    // Operators
    Matrix4 operator *(float s)
    {
        Matrix4 result;
        for(int i = 0; i < 16; i++)
        {
            result.elements[i] = this->elements[i]*i;
        }
        return result;
    }
    Matrix4 operator *(Matrix4 rightSideMatrix)
    {
        Matrix4 result;
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                result.elements[i*4+j] = this->elements[i*4+0]*rightSideMatrix.elements[j*1 ]
                                        +this->elements[i*4+1]*rightSideMatrix.elements[j+4 ]
                                        +this->elements[i*4+2]*rightSideMatrix.elements[j+8 ]
                                        +this->elements[i*4+3]*rightSideMatrix.elements[j+12];
            }
        }
        return result;
    }
    Vector4 operator *(Vector4 v)
    {
        return Vector4(
            te[0] * v.x + te[1] * v.y + te[2] * v.z + te[3] * v.w,
            te[4] * v.x + te[5] * v.y + te[6] * v.z + te[7] * v.w,
            te[8] * v.x + te[9] * v.y + te[10] * v.z + te[11] * v.w,
            te[12] * v.x + te[13] * v.y + te[14] * v.z + te[15] * v.w
        );
    }
    Vector4 getPosition()
    {
        return Vector4(this->getElement(0, 3), this->getElement(1, 3), this->getElement(2, 3), this->getElement(3, 3));
    }
    void log()
    {
        printf("[\n");
        for(int i = 0; i < 16; i++)
        {
            printf("%f, ", this->elements[i]);
            if((i+1)%4==0)
                printf("\n");
        }
        printf("]\n");
    }
};