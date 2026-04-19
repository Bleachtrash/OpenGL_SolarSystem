#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <shader_vars.hpp>
#include <math/Matrix4.hpp>
#include <HelperFuncs.hpp>
#include <ModelObject.hpp>
#include <Quad.hpp>
#include <Cube.hpp>

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

    Uniforms uniforms(shader); Attributes attribs(shader);

    ModelObject earth(uniforms, attribs, "./Models/sphere.obj", "./Textures/Earth.png", GL_RGBA);
    earth.worldMatrix = Matrix4()*Matrix4().makeTranslation(0, 0, 0)*Matrix4().makeScale(2, 2, 2);
    // Create camera and projection matrix
    Camera camera;
    Matrix4 projectionMatrix;
    projectionMatrix.makePerspective(45, 1, 0.1, 1000);

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
    while(!glfwWindowShouldClose(window))
    {
        // obj.worldMatrix = Matrix4().makeRotationX(23.45)*Matrix4().makeRotationY(fmod(glfwGetTime()*100, 360))*Matrix4().makeScale(5, 5, 5);
        // // Update our camera object
        camera.update();
        // camera.yawDegrees += 0.5;
        camera.yawDegrees *= camera.yawDegrees<360;
        // // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(shader, "uTime"), glfwGetTime());
        // Render out objects
        earth.render(camera, projectionMatrix, shader);

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
    }
    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}