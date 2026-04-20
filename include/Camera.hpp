#pragma once

#include <math/Matrix4.hpp>

float min(float a, float b)
{
    return a<b? a : b;
}
float max(float a, float b)
{
    return a>b? a : b;
}

struct Camera
{
    Matrix4 cameraWorldMatrix;
    Vector4 cameraTarget;
    Vector4 lookAtTarget;
    float yawDegrees;
    float pitchDegrees;
    float minDistance;
    float maxDistance;
    float zoomScale;

    bool isDragging;
    int lastMouseX, lastMouseY;
    Camera()
    {
        lastMouseX = 0;
        lastMouseY = 0;
        isDragging = false;
        this->yawDegrees = 0;
        this->pitchDegrees = 0;
        this->minDistance = 1;
        this->maxDistance = 30;
        this->zoomScale = 2.0;
    }
    Matrix4 getViewMatrix()
    {
        return this->cameraWorldMatrix.clone().inverse();
    }
    Vector4 getPosition()
    {
            return Vector4(
                this->cameraWorldMatrix.elements[3],
                this->cameraWorldMatrix.elements[7],
                this->cameraWorldMatrix.elements[11],
                1.0
            );    
    }
    void update()
    {
        this->pitchDegrees = min(max(this->pitchDegrees, -85), 85);
        Vector4 tether = Vector4(0, 0, this->minDistance + (this->maxDistance - this->minDistance)*zoomScale, 0);
        Matrix4 yaw = Matrix4().makeRotationY(this->yawDegrees);
        Matrix4 pitch = Matrix4().makeRotationX(this->pitchDegrees);
        
        Vector4 transformedTether = pitch*tether;
        transformedTether = yaw*transformedTether;
        
        Vector4 position = cameraTarget+transformedTether;
        this->lookAt(position, Vector4(lookAtTarget.x, lookAtTarget.y, lookAtTarget.z, 1));
    }
    Camera lookAt(Vector4 eyePos, Vector4 targetPos)
    {
        Vector4 worldUp = Vector4(0, 1, 0, 0);

        this->cameraWorldMatrix.makeIdentity();
        Vector4 forward = (targetPos-eyePos).Normalized();
        Vector4 right = forward.cross(worldUp).Normalized();
        Vector4 up = right.cross(forward).Normalized();

        #define e this->cameraWorldMatrix.elements
        e[0] = right.x; e[1] = up.x; e[2] = -forward.x; e[3] = eyePos.x;
        e[4] = right.y; e[5] = up.y; e[6] = -forward.y; e[7] = eyePos.y;
        e[8] = right.z; e[9] = up.z; e[10] = -forward.z; e[11] = eyePos.z;
        e[12] = 0; e[13] = 0; e[14] = 0; e[15] = 1;

        return *this;
    }
    void onMouseDown(double currMouseX, double currMouseY)
    {
        isDragging = true;
        this->lastMouseX = currMouseX;
        this->lastMouseY = currMouseY;
    }
    void onMouseMove(double currMouseX, double currMouseY)
    {
        if(isDragging)
        {
            this->yawDegrees -= (currMouseX - lastMouseX) * 0.5;
            this->pitchDegrees -= (currMouseY - lastMouseY) * 0.5;

            this->pitchDegrees = min(this->pitchDegrees, 85);
            this->pitchDegrees = max(this->pitchDegrees, -85);

            this->lastMouseX = currMouseX;
            this->lastMouseY = currMouseY;
        }
    }
    void onMouseUp(double currMouseX, double currMouseY)
    {
        isDragging = false;
    }

    void onScroll(double yoffset)
    {
        this->zoomScale -= yoffset/10;
        this->zoomScale = max(this->zoomScale, 0);
    }
};