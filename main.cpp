#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <shader_vars.hpp>
#include <math/Matrix4.hpp>
#include <HelperFuncs.hpp>
#include <Planet.hpp>
#include <ModelObject.hpp>
#include <Quad.hpp>
#include <Cube.hpp>
#include <vector>
// I'm pretty sure there's a memory leak somehwere... no clue where tho :/
int main()
{
    glfwInit();

    // Set up GLFW
    GLFWwindow *window = InitStuff(1000, 1000, "OpenGL");
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    // Create shader program with vertex and fragment shaders
    GLuint shader = LoadShaders("./shaders/phong-vs.glsl", "./shaders/phong-fs.glsl");
    GLuint unlitShader = LoadShaders("./shaders/unlit-vs.glsl", "./shaders/unlit-fs.glsl");
    GLuint transparentShader = LoadShaders("./Shaders/transparent-vs.glsl", "./shaders/transparent-fs.glsl");
    Uniforms uniforms(shader); Attributes attribs(shader);
    Uniforms unlitUniforms(unlitShader); Attributes unlitAttribs(unlitShader);
    Uniforms transUniforms(transparentShader); Attributes transAttribs(transparentShader);
    ModelObject sun(unlitUniforms, unlitAttribs, "./Models/sphere.obj", "./Textures/Sun.png", GL_RGBA);
    // sun.worldMatrix = Matrix4()*Matrix4().makeScale(0.0926667, 0.0926667, 0.0926667);
    sun.worldMatrix = Matrix4()*Matrix4().makeTranslation(0, 0, 0)*Matrix4().makeScale(0.927*10, 0.927*10, 0.927*10);
    // printf("Sun\n");
    Planet murcury(uniforms, attribs, "./Textures/Murcury.png", 0.003*100, 39.93, 10.0/88, 1.0/59, &sun.worldMatrix);
    Planet venus(uniforms, attribs, "./Textures/Venus.png", 0.00807*100, 72, 10.0/225, 1.0/243, &sun.worldMatrix);
    Planet earth(uniforms, attribs, "./Textures/Earth.png", "./Textures/EarthNight.png", 0.0085*100, 100, 10.0/365, 1.0/1, 24, &sun.worldMatrix);
    Planet earthCloud(transUniforms, transAttribs, "./Textures/EarthCloud.png",  0.0086*100, 0, 10.0/365, 0.9/1, 24, &earth.obj.worldMatrix);
    Planet moon(uniforms, attribs, "./Textures/Moon.png", 0.0023*100, 2.56, 10.0/27, 1.0/27, &earth.obj.worldMatrix);
    Planet mars(uniforms, attribs, "./Textures/Mars.png", 0.00452*100, 152, 10.0/686.2, 1.0/1.03, &sun.worldMatrix);
    Planet jupiter(uniforms, attribs, "./Textures/Jupiter.png", 0.0953*100, 520-200, 10.0/4343.5, 1.0/0.5, &sun.worldMatrix);
    Planet saturn(uniforms, attribs, "./Textures/Saturn.png", 0.0803*100, 955.33-200, 10.0/10585, 1.0/0.5, &sun.worldMatrix);
    Quad rings(unlitUniforms, unlitAttribs, "./Textures/Rings.png");
    Planet uranus(uniforms, attribs, "./Textures/Uranus.png", 0.0341*100, 1913.33-200, 10.0/30660, 1.0/0.75, &sun.worldMatrix);
    Planet neptune(uniforms, attribs, "./Textures/Neptune.png", 0.033*100, 3000-200, 10.0/90225, 1.0/0.667, &sun.worldMatrix);

    std::vector<Planet*> planets;
    planets.push_back(&murcury);
    planets.push_back(&venus);
    planets.push_back(&earth);
    planets.push_back(&moon);
    planets.push_back(&mars);
    planets.push_back(&jupiter);
    planets.push_back(&saturn);
    planets.push_back(&uranus);
    planets.push_back(&neptune);

    // Create camera and projection matrix
    Camera camera;
    ModelObject *cameraTargetPlanet = &sun;
    Matrix4 projectionMatrix;
    projectionMatrix.makePerspective(45, 1, 0.1, 100000);

    // Disable sticky inputs
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
    // Set up scroll event call back and mouse movement call back
    glfwSetWindowUserPointer(window, &camera);  // Create a pointer we can use in the callback func
    // Set the scroll event callback to an in-line function
    glfwSetScrollCallback(window, [](GLFWwindow* window,double xoffset, double yoffset) { 
        Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
        camera->onScroll(yoffset); 
    });
    // Set mouse event callback to an in-line function
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double mouseX, double mouseY){
        Camera *camera = (Camera*)glfwGetWindowUserPointer(window);
        camera->onMouseMove(mouseX, mouseY);
    });

    // Create vars for mouse x and y position
    double mouseX = 0, mouseY = 0;
    // Main loop
    double NOW = glfwGetTime();
    double LAST = 0;
    double deltaTime;
    while(!glfwWindowShouldClose(window))
    {
        LAST = NOW;
        NOW = glfwGetTime();
        deltaTime = (NOW-LAST)*100;
        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glUniform1f(glGetUniformLocation(shader, "uTime"), glfwGetTime());
        // glUniform3f(glGetUniformLocation(shader, "uLightPos"), sun.worldMatrix.getElement(0, 3), sun.worldMatrix.getElement(1, 3), sun.worldMatrix.getElement(2, 3));
        
        // Render out objects
        sun.render(camera, projectionMatrix, unlitShader);

        for(int i = 0; i < planets.size(); i++)
        {
            planets[i]->render(camera, projectionMatrix, shader);
        }
        earthCloud.render(camera, projectionMatrix, transparentShader);
        rings.worldMatrix = Matrix4().makeTranslation(saturn.obj.worldMatrix.getPosition())*Matrix4().makeRotationX(-80)*Matrix4().makeScale(15, 15, 15);
        rings.render(camera, projectionMatrix, unlitShader);
        // // Update our camera object
        camera.update();
        camera.cameraTarget = cameraTargetPlanet->worldMatrix.getPosition();
        camera.lookAtTarget = camera.cameraTarget;

        // camera.yawDegrees *= camera.yawDegrees<360;

        // Render current frame
        glfwSwapBuffers(window);
        
        // Poll events
        glfwPollEvents();

        // Set mouse x and y variables
        glfwGetCursorPos(window, &mouseX, &mouseY);
        // Check if left mouse button was pressed
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            camera.onMouseDown(mouseX, mouseY);
        }
        // Check if left mouse button was released
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            camera.onMouseUp(mouseX, mouseY);
        }
        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            cameraTargetPlanet = &sun;
        }
        else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            cameraTargetPlanet = &murcury.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            cameraTargetPlanet = &venus.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            cameraTargetPlanet = &earth.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            cameraTargetPlanet = &mars.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            cameraTargetPlanet = &jupiter.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            cameraTargetPlanet = &saturn.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            cameraTargetPlanet = &uranus.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            cameraTargetPlanet = &neptune.obj;
        }
        else if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        {
            cameraTargetPlanet = &moon.obj;
        }
    }
    // Cleanup
    glDeleteShader(shader);
    glDeleteShader(unlitShader);
    glDeleteShader(transparentShader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}