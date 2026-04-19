#pragma once

#include <Quad.hpp>

struct Cube
{
    Quad t, d, l, r, f, b;
    Matrix4 worldMatrix;

    Cube(Uniforms uniforms, Attributes attribs)
    {
        // Create quads for all 6 sides
        this->t = Quad(uniforms, attribs); this->d = Quad(uniforms, attribs); this->l = Quad(uniforms, attribs);
        this->r = Quad(uniforms, attribs); this->f = Quad(uniforms, attribs); this->b = Quad(uniforms, attribs);        
    }
    Cube(Uniforms uniforms, Attributes attribs, char *image)
    {
        // Create quads for all 6 sides
        this->t = Quad(uniforms, attribs, image); this->d = Quad(uniforms, attribs, image); this->l = Quad(uniforms, attribs, image);
        this->r = Quad(uniforms, attribs, image); this->f = Quad(uniforms, attribs, image); this->b = Quad(uniforms, attribs, image);
    }
    void Reload()
    {
        // Set every quad worl matrix to form a cube around this object's world matrix
        this->t.worldMatrix = this->worldMatrix*Matrix4().makeTranslation(0, 1, 0)*Matrix4().makeRotationX(-90);
        this->d.worldMatrix = this->worldMatrix*Matrix4().makeTranslation(0, -1, 0)*Matrix4().makeRotationX(90);

        this->l.worldMatrix = this->worldMatrix*Matrix4().makeTranslation(-1, 0, 0)*Matrix4().makeRotationY(-90);
        this->r.worldMatrix = this->worldMatrix*Matrix4().makeTranslation(1, 0, 0)*Matrix4().makeRotationY(90);

        this->f.worldMatrix = this->worldMatrix*Matrix4().makeTranslation(0, 0, 1)*Matrix4().makeRotationY(0);
        this->b.worldMatrix = this->worldMatrix*Matrix4().makeTranslation(0, 0, -1)*Matrix4().makeRotationY(180);
    }
    void render(Camera camera, Matrix4 projectionMatrix, int shader)
    {
        // Set every quad's worl matrix
        this->Reload();
        // Render each quad
        this->t.render(camera, projectionMatrix, shader);
        this->d.render(camera, projectionMatrix, shader);
        this->l.render(camera, projectionMatrix, shader);
        this->r.render(camera, projectionMatrix, shader);
        this->f.render(camera, projectionMatrix, shader);
        this->b.render(camera, projectionMatrix, shader);
    }
};