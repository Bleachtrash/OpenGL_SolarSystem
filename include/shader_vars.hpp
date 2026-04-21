#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Uniforms
{
    int projectionMatrixUniform;
    int viewMatrixUniform;
    int worldMatrixUniform;
    int textureUniform;
    int cameraPositionUniform;
    Uniforms()
    {
        this->projectionMatrixUniform = 0;
        this->viewMatrixUniform = 0;
        this->worldMatrixUniform = 0;
        this->textureUniform = 0;
        this->cameraPositionUniform = 0;
    }
    Uniforms(int shader_program)
    {
        this->projectionMatrixUniform = glGetUniformLocation(shader_program, "uProjectionMatrix");
        this->viewMatrixUniform = glGetUniformLocation(shader_program, "uViewMatrix");
        this->worldMatrixUniform = glGetUniformLocation(shader_program, "uWorldMatrix");
        this->textureUniform = glGetUniformLocation(shader_program, "uTexture");
        this->cameraPositionUniform = glGetUniformLocation(shader_program, "uCameraPosition");
    }
};

struct Attributes
{
    int vertexPositionAttrib;
    int vertexNormalAttrib;
    int texCoordsAttrib;
    Attributes()
    {
        this->vertexPositionAttrib = 0;
        this->vertexNormalAttrib = 0;
        this->texCoordsAttrib = 0;
    }
    Attributes(int shader_program)
    {
        this->vertexPositionAttrib = glGetAttribLocation(shader_program, "aVertexPosition");
        this->vertexNormalAttrib = glGetAttribLocation(shader_program, "aVertexNormal");
        this->texCoordsAttrib = glGetAttribLocation(shader_program, "aTexCoords");
        
    }
};