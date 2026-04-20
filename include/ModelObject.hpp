#pragma once
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include <math/Matrix4.hpp>
#include <math/Vector3.hpp>
#include <stb/stb_image.h>
#include <Camera.hpp>
#include <shader_vars.hpp>

struct ModelObject
{
    GLfloat *vertices;
    GLuint vertexBuffer;    
    int num_vertices;
    GLint *indices;
    GLuint indexBuffer;
    int num_indices;
    GLfloat *normals;
    GLfloat *texCoords;
    GLuint texCoordBuffer;
    Matrix4 worldMatrix;
    GLuint normalBuffer;
    GLuint texture;

    Uniforms uniforms;
    Attributes attribs;

    ModelObject(Uniforms uniforms, Attributes attribs, char *ModelPath)
    {
        this->texture = 0;
        this->uniforms = uniforms;
        this->attribs = attribs;
        // Check file type
        char *file_type = new char[4];
        strcpy(file_type, ModelPath+strlen(ModelPath)-4);
        if(strcmp(file_type, ".obj"))
        {
            printf("Error: %s is not an .obj file!\n", ModelPath);
            return;
        }
        // Open file
        std::ifstream file(ModelPath);
        if(!file)
        {
            printf("Error: %s no such file!\n", ModelPath);
            return;
        }

        num_vertices = 0;
        num_indices = 0;
        int num_vertexTextures = 0;
        int num_vertexNormals = 0;
        std::string line;
        int startV = 0, startI = 0, startVT = 0, startVN = 0;
        // Read data from file
        for(int i = 0; getline(file, line); i++)
        {
            // Get the number of vertices and where they start
            if(line.substr(0, 2) == "v ")
            {
                if(!startV)
                    startV = i;
                num_vertices += 3;
            }
            // Get the number of vertex normals and where they start
            else if(line.substr(0, 2) == "vn")
            {
                if(!startVN)
                    startVN = i;
                num_vertexNormals += 3;
            }
            // Get the start of the vertex texture list
            else if(line.substr(0, 2) == "vt")
            {
                if(!startVT)
                    startVT = i;
                num_vertexTextures += 2;
            }
            // Get the number of indices and where they start
            else if(line.substr(0, 2) == "f ")
            {
                if(!startI)
                    startI = i;
                num_indices += 3;
            }
        }
        this->vertices = new GLfloat[num_vertices];
        GLfloat *texCoordsTemp = new GLfloat[num_vertexTextures];
        GLfloat *vertexNormalTemp = new GLfloat[num_vertexNormals];
        this->indices = new GLint[num_indices];
        GLint *textureIndices = new GLint[num_indices];
        GLint *normalIndices = new GLint[num_indices];
        file.clear();
        file.seekg(0);
        for(int i = 0; std::getline(file, line); i++)
        {
            if(i >= startI && i < startI+num_indices/3)
            {
                // Read indices
                std::string lineElements[4];
                char* t = strtok((char*)line.c_str(), " ");
                for(int j = 0; j < 4; j++)
                {
                    lineElements[j] = t;
                    t = strtok(nullptr, " ");
                }
                for(int j = 1; j < 4; j++)
                {
                    // Get the first element of each face list(vertex_index)
                    t = strtok((char*)lineElements[j].c_str(), "/");
                    this->indices[(i-startI)*3+j-1] = std::stoi(t)-1;
                    // Get the second element of each face list(vertex_texture_pos)
                    t = strtok(nullptr, "/");
                    textureIndices[(i-startI)*3+j-1] = std::stoi(t)-1;
                    // Get the third element of each face list(vertex_texture)
                    t = strtok(nullptr, "/");
                    normalIndices[(i-startI)*3+j-1] = std::stoi(t)-1;
                }
            }
            else if (i >= startVN && i < startVN+num_vertexNormals/3)
            {
                // Read vertex normal dir
                std::string lineElements[4];
                char* t = strtok((char*)line.c_str(), " ");
                for(int j = 0; t != nullptr; j++)
                {
                    lineElements[j] = t;
                    t = strtok(nullptr, " ");
                }
                for(int j = 1; j < 4; j++)
                {
                    vertexNormalTemp[(i-startVN)*3+j-1] = std::stof(lineElements[j]);
                }
            }
            else if(i >= startVT && i < startVT+num_vertexTextures/2)
            {
                // Read vertex texcoords
                std::string lineElements[3];
                char* t = strtok((char*)line.c_str(), " ");
                for(int j = 0; t != nullptr; j++)
                {
                    lineElements[j] = t;
                    t = strtok(nullptr, " ");
                }
                for(int j = 1; j < 3; j++)
                {
                    texCoordsTemp[(i-startVT)*2+j-1] = std::stof(lineElements[j]);
                }
            }
            else if(i >= startV && i < startV+num_vertices/3)
            {
                // Read vertices
                std::string lineElements[4];
                char* t = strtok((char*)line.c_str(), " ");
                for(int j = 0; t != nullptr; j++)
                {
                    lineElements[j] = t;
                    t = strtok(nullptr, " ");
                }
                for(int j = 1; j < 4; j++)
                {
                    this->vertices[(i-startV)*3+j-1] = std::stof(lineElements[j]);
                }
            }
        }
        // Create, bind, and set vertex array buffer...
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*num_vertices, vertices, GL_STATIC_DRAW);

        // Create, bind, and set index array buffer...
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*num_indices, indices, GL_STATIC_DRAW);

        // Set texCoords
        this->texCoords = new GLfloat[num_indices*2];
        for(int i = 0; i < num_indices; i+=3)
        {
            int ia = 2*textureIndices[i];
            int ib = 2*textureIndices[i+1];
            int ic = 2*textureIndices[i+2];
            float iax = texCoordsTemp[ia], iay = texCoordsTemp[ia+1];
            float ibx = texCoordsTemp[ib], iby = texCoordsTemp[ib+1];
            float icx = texCoordsTemp[ic], icy = texCoordsTemp[ic+1];

            ia = 2*indices[i];
            ib = 2*indices[i+1];
            ic = 2*indices[i+2];
            this->texCoords[ia] = iax; this->texCoords[ia+1] = iay;
            this->texCoords[ib] = ibx; this->texCoords[ib+1] = iby;
            this->texCoords[ic] = icx; this->texCoords[ic+1] = icy;
        }

        // Create, bind, and set the vertex texture buffer
        glGenBuffers(1, &texCoordBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*num_indices*2, this->texCoords, GL_STATIC_DRAW);
        // Set normals
        this->normals = new GLfloat[num_indices*3];
        for(int i = 0; i < num_indices; i+=3)
        {
            int ia = 3*normalIndices[i];
            int ib = 3*normalIndices[i+1];
            int ic = 3*normalIndices[i+2];
            Vector3 iav = Vector3(vertexNormalTemp[ia], vertexNormalTemp[ia+1], vertexNormalTemp[ia+2]);
            Vector3 ibv = Vector3(vertexNormalTemp[ib], vertexNormalTemp[ib+1], vertexNormalTemp[ib+2]);
            Vector3 icv = Vector3(vertexNormalTemp[ic], vertexNormalTemp[ic+1], vertexNormalTemp[ic+2]);

            // iav = Vector3(0.0, 0.0, 1.0);

            ia = 3*indices[i];
            ib = 3*indices[i+1];
            ic = 3*indices[i+2];
            normals[ia] = iav.x;
            normals[ia+1] = iav.y;
            normals[ia+2] = iav.z;

            normals[ib] = ibv.x;
            normals[ib+1] = ibv.y;
            normals[ib+2] = ibv.z;

            normals[ic] = icv.x;
            normals[ic+1] = icv.y;
            normals[ic+2] = icv.z;
        }

        // Create, bind, and set normal array buffer...
        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*num_indices*3, normals, GL_STATIC_DRAW);
    }
    ModelObject(Uniforms uniforms, Attributes attribs, char* model_path, char* texture_file, GLenum format) : ModelObject(uniforms, attribs, model_path)
    {
        // Create and bind the texture buffer
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        // Get the image data using stb_image.h
        int width, height, nrChannels;
        unsigned char *data = stbi_load(texture_file, &width, &height, &nrChannels, 0);
        if(!data)
            printf("IMAGE FAILED TO LOAD!!\n");
        // Send the image data to GL_TEXTURE_2D
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        // Set texture filtering method
        glGenerateMipmap(GL_TEXTURE_2D);    // Generate image mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set minification filtering to tri-linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set magnification filterint to tri-linear
    }
    void render(Camera camera, Matrix4 projectionMatrix, GLuint shader)
    {
        glUseProgram(shader);

        // Set shader uniforms
        glUniformMatrix4fv(uniforms.worldMatrixUniform, 1, true, this->worldMatrix.elements);
        glUniformMatrix4fv(uniforms.viewMatrixUniform, 1, true, camera.getViewMatrix().elements);
        glUniformMatrix4fv(uniforms.projectionMatrixUniform, 1, true, projectionMatrix.elements);
        glUniform3f(uniforms.cameraPositionUniform, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
        // Activate and bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        
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
        // Set aTexCoords
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
        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(attribs.vertexPositionAttrib);
        glDisableVertexAttribArray(attribs.texCoordsAttrib);
        glDisableVertexAttribArray(attribs.vertexNormalAttrib);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};