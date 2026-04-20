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

    Uniforms uniforms(shader); Attributes attribs(shader);
    Uniforms unlitUniforms(unlitShader); Attributes unlitAttribs(unlitShader);
    ModelObject sun(unlitUniforms, unlitAttribs, "./Models/sphere.obj", "./Textures/Sun.png", GL_RGBA);
    // sun.worldMatrix = Matrix4()*Matrix4().makeScale(0.0926667, 0.0926667, 0.0926667);
    sun.worldMatrix = Matrix4()*Matrix4().makeTranslation(0, 0, 0)*Matrix4().makeScale(0.927*10, 0.927*10, 0.927*10);

    ModelObject murcury(uniforms, attribs, "./Models/sphere.obj", "./Textures/Murcury.png", GL_RGBA);
    murcury.worldMatrix = Matrix4()*Matrix4().makeTranslation(39.93, 0, 0)*Matrix4().makeScale(0.003*100, 0.003*100, 0.003*100);

    ModelObject venus(uniforms, attribs, "./Models/sphere.obj", "./Textures/Venus.png", GL_RGBA);
    venus.worldMatrix = Matrix4()*Matrix4().makeTranslation(72, 0, 0)*Matrix4().makeScale(0.008069*100, 0.008069*100, 0.008069*100);

    ModelObject earth(uniforms, attribs, "./Models/sphere.obj", "./Textures/Earth.png", GL_RGBA);
    earth.worldMatrix = Matrix4()*Matrix4().makeTranslation(100, 0, 0)*Matrix4().makeScale(0.0085*100, 0.0085*100, 0.0085*100);

    ModelObject mars(uniforms, attribs, "./Models/sphere.obj", "./Textures/Mars.png", GL_RGBA);
    mars.worldMatrix = Matrix4()*Matrix4().makeTranslation(152, 0, 0)*Matrix4().makeScale(0.00452*100, 0.00452*100, 0.00452*100);

    ModelObject jupiter(uniforms, attribs, "./Models/sphere.obj", "./Textures/Jupiter.png", GL_RGBA);
    jupiter.worldMatrix = Matrix4()*Matrix4().makeTranslation(520, 0, 0)*Matrix4().makeScale(0.953*10, 0.953*10, 0.953*10);

    ModelObject saturn(uniforms, attribs, "./Models/sphere.obj", "./Textures/Saturn.png", GL_RGBA);
    saturn.worldMatrix = Matrix4()*Matrix4().makeTranslation(955.333, 0, 0)*Matrix4().makeScale(0.0803*100, 0.0803*100, 0.0803*100);

    ModelObject uranus(uniforms, attribs, "./Models/sphere.obj", "./Textures/Uranus.png", GL_RGBA);
    uranus.worldMatrix = Matrix4()*Matrix4().makeTranslation(1913.333, 0, 0)*Matrix4().makeScale(0.0341*100, 0.0341*100, 0.0341*100);

    ModelObject neptune(uniforms, attribs, "./Models/sphere.obj", "./Textures/Neptune.png", GL_RGBA);
    neptune.worldMatrix = Matrix4()*Matrix4().makeTranslation(3000, 0, 0)*Matrix4().makeScale(0.033*100, 0.033*100, 0.033*100);

    std::vector<ModelObject> planets;
    planets.push_back(murcury);
    planets.push_back(venus);
    planets.push_back(earth);
    planets.push_back(mars);
    planets.push_back(jupiter);
    planets.push_back(saturn);
    planets.push_back(uranus);
    planets.push_back(neptune);

    // Create camera and projection matrix
    Camera camera;
    ModelObject cameraTargetPlanet = sun;
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
    while(!glfwWindowShouldClose(window))
    {
        // Update our camera object
        camera.update();
        camera.cameraTarget = cameraTargetPlanet.worldMatrix.getPosition();
        camera.lookAtTarget = camera.cameraTarget;
        camera.yawDegrees *= camera.yawDegrees<360;
        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glUniform1f(glGetUniformLocation(shader, "uTime"), glfwGetTime());
        // glUniform3f(glGetUniformLocation(shader, "uLightPos"), sun.worldMatrix.getElement(0, 3), sun.worldMatrix.getElement(1, 3), sun.worldMatrix.getElement(2, 3));
        
        // Render out objects
        sun.render(camera, projectionMatrix, unlitShader);

        for(ModelObject planet : planets)
        {
            planet.render(camera, projectionMatrix, shader);
        }
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
            cameraTargetPlanet = sun;
        }
        else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            cameraTargetPlanet = murcury;
        }
        else if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            cameraTargetPlanet = venus;
        }
        else if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            cameraTargetPlanet = earth;
        }
        else if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            cameraTargetPlanet = mars;
        }
        else if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            cameraTargetPlanet = jupiter;
        }
        else if(glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            cameraTargetPlanet = saturn;
        }
        else if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            cameraTargetPlanet = uranus;
        }
        else if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            cameraTargetPlanet = neptune;
        }
    }
    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}