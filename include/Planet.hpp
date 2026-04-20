#pragma once
#include <Math/Matrix4.hpp>
#include <ModelObject.hpp>
#include <shader_vars.hpp>
#include <random>

struct Planet
{
    ModelObject obj;
    Matrix4 *orbit_obj;
    float diameter;
    float orbit_dist;
    float axis_tilt;
    float orbit_degrees;
    float rotation_degrees;
    float orbit_speed;
    float rotation_speed;

    Planet(Uniforms uniforms, Attributes attribs, char *texture_file, float diameter, float orbit_dist, float orbit_speed, float rotation_speed, float axis_tilt, Matrix4 *orbit_obj)
    {
        this->obj = ModelObject(uniforms, attribs, "./Models/sphere.obj", texture_file, GL_RGBA);
        this->diameter = diameter;
        this->orbit_dist = orbit_dist;
        this->orbit_obj = orbit_obj;
        this->orbit_degrees = rand()%365;
        this->rotation_degrees = 0;
        this->orbit_speed = orbit_speed;
        this->rotation_speed = rotation_speed;
        this->axis_tilt = axis_tilt;
    }
    Planet(Uniforms uniforms, Attributes attribs, char *texture_file, float diameter, float orbit_dist, float orbit_speed, float rotation_speed, Matrix4 *orbit_obj)
    : Planet(uniforms, attribs, texture_file, diameter, orbit_dist, orbit_speed, rotation_speed, 0, orbit_obj)
    {

    }
    void Update()
    {
        Vector4 orbit_center = orbit_obj->getPosition();
        orbit_degrees += orbit_speed;
        rotation_degrees += rotation_speed;
        Matrix4 orbit = Matrix4().makeRotationY(orbit_degrees);
        Matrix4 rotation = Matrix4().makeRotationY(rotation_degrees)*Matrix4().makeRotationX(axis_tilt);
        Matrix4 translation1 = Matrix4().makeTranslation(orbit_center.x, orbit_center.y, orbit_center.z);
        Matrix4 translation2 = Matrix4().makeTranslation(orbit_dist, 0, 0);
        Matrix4 scale = Matrix4().makeScale(diameter, diameter, diameter);
        obj.worldMatrix = Matrix4()*translation1*orbit*translation2*rotation*scale;
    }
    void render(Camera camera, Matrix4 projectionMatrix, GLuint shader)
    {
        obj.render(camera, projectionMatrix, shader);
        this->Update();
    }
};