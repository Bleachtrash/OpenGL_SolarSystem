#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math/Matrix4.hpp>
#include <math/Vector3.hpp>
#include <Camera.hpp>
#include <shader_vars.hpp>

struct Quad
{
    GLfloat *vertices;
    GLuint vertexBuffer;    
    GLuint *indices;
    GLuint indexBuffer;
    GLfloat *normals;
    GLuint normalBuffer;
    GLfloat *texCoords;
    GLuint texCoordBuffer;
    int num_vertices;
    Matrix4 worldMatrix;
    GLuint texture;

    Uniforms uniforms;
    Attributes attribs;

    Quad(){}
    Quad(Uniforms uniforms, Attributes attribs)
    {
        this->texture = 0;
        this->uniforms = uniforms;
        this->attribs = attribs;
        num_vertices = 4*3;

        this->vertices = new GLfloat[num_vertices] {
            -1.0, -1.0,  0.0,
             1.0, -1.0,  0.0,
            -1.0,  1.0,  0.0,
             1.0,  1.0,  0.0
        };
        // Create, bind, and set vertex array buffer...
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*num_vertices, vertices, GL_STATIC_DRAW);

        this->indices = new GLuint[6] {0, 1, 2, 2, 1, 3};
        // Create, bind, and set index array buffer...
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*6, indices, GL_STATIC_DRAW);
        // Create, bind and set normal array buffer...
        this->normals = new GLfloat[num_vertices] {
            0.0,  0.0,  1.0,
            0.0,  0.0,  1.0,
            0.0,  0.0,  1.0,
            0.0,  0.0,  1.0
        };
        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*num_vertices, normals, GL_STATIC_DRAW);
        this->texCoords = new GLfloat[8] {
            0, 0,
            1, 0,
            0, 1,
            1, 1
        };
        glGenBuffers(1, &texCoordBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, this->texCoords, GL_STATIC_DRAW);
    }
    Quad(Uniforms uniforms, Attributes attribs, char *texture_file) : Quad(uniforms, attribs)
    {
        // Create and bind the texture buffer
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        // Get the image data using stb_image.h
        int width, height, nrChannels;
        unsigned char *data = stbi_load(texture_file, &width, &height, &nrChannels, 0);
        // Send the image data to GL_TEXTURE_2D
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        // Set texture filtering method
        glGenerateMipmap(GL_TEXTURE_2D);    // Generate image mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set minification filtering to tri-linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set magnification filterint to tri-linear
    }
    void render(Camera camera, Matrix4 projectionMatrix, int shader)
    {
        glUseProgram(shader);
        // Set shader uniforms
        glUniformMatrix4fv(uniforms.worldMatrixUniform, 1, true, this->worldMatrix.elements);
        glUniformMatrix4fv(uniforms.viewMatrixUniform, 1, true, camera.getViewMatrix().elements);
        glUniformMatrix4fv(uniforms.projectionMatrixUniform, 1, true, projectionMatrix.elements);
        glUniform3f(uniforms.cameraPositionUniform, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

        if(this->texture)
        {
            // Activate and bind texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->texture);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);

        
            glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
        // Sets the aVertexPosition
        glVertexAttribPointer(
            attribs.vertexPositionAttrib,
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        glEnableVertexAttribArray(attribs.vertexPositionAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, this->texCoordBuffer);
        glVertexAttribPointer(
            attribs.texCoordsAttrib,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glEnableVertexAttribArray(attribs.texCoordsAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
        // Sets aVertexNormal
        glVertexAttribPointer(
            attribs.vertexNormalAttrib,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glEnableVertexAttribArray(attribs.vertexNormalAttrib);


        // Draw the elements
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(attribs.vertexPositionAttrib);
        glDisableVertexAttribArray(attribs.texCoordsAttrib);
        glDisableVertexAttribArray(attribs.vertexNormalAttrib);

        if(this->texture)
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Quad()
    {
        this->indices = nullptr;
        free(indices);
        glDeleteBuffers(1, &this->indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        this->vertices = nullptr;
        free(this->vertices);
        glDeleteBuffers(1, &this->vertexBuffer);

        this->normals = nullptr;
        free(this->normals);
        glDeleteBuffers(1, &this->normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};