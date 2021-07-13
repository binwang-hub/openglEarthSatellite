//
//  main.cpp
//  openglEarthSatellite
//
//  Created by Wang Bin on 2021/7/9.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <glad/glad.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "vendor/stb_image/stb_image.h"

#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "model.h"
#include "circle.h"

// Globals
bool animation = false;

// Timing
float lastFrame = 0.0f;
float frameToggled = 0.0f;
float timeSinceLastToggle = 1.0f;

// Some settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

float earthOrbitRadius = 100.0f;
float moonOrbitRadius = 20.0f;
glm::vec3 sunPos = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 30.0f));
float cameraOrbitRadius = 30.0f;
float rotateAngle = 1.0f;

// Mouse Input
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseInput(GLFWwindow * window, double xpos, double ypos);
void scrollInput(GLFWwindow * window, double xoffset, double yoffset);
void keyboardInput(GLFWwindow * window, float deltaTime);


int main()
{
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Satell & Earth System Based on OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollInput);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader planetShader("/Users/wangbin/Desktop/openglEarthSatellite/res/shaders/planet.vs", "/Users/wangbin/Desktop/openglEarthSatellite/res/shaders/planet.fs");
    Shader sunShader("/Users/wangbin/Desktop/openglEarthSatellite/res/shaders/sun.vs", "/Users/wangbin/Desktop/openglEarthSatellite/res/shaders/sun.fs");

    // Load the models
    Model Sun("/Users/wangbin/Desktop/openglEarthSatellite/res/models/Planet/planet.obj");
    Model Earth("/Users/wangbin/Desktop/openglEarthSatellite/res/models/Earth/Globe.obj");
    Model Moon("/Users/wangbin/Desktop/openglEarthSatellite/res/models/Rock/rock.obj");

    Circle EarthOrbitCircle(sunPos, earthOrbitRadius, glm::vec3(0.0f, 1.0f, 1.0f), 3000);
    Circle MoonOrbitCircle(earthPos, moonOrbitRadius, glm::vec3(1.0f, 1.0f, 0.0f), 3000);
    Skybox skyBox("/Users/wangbin/Desktop/openglEarthSatellite/res/images/top.png", "/Users/wangbin/Desktop/openglEarthSatellite/res/images/bottom.png",                         "/Users/wangbin/Desktop/openglEarthSatellite/res/images/left.png", "/Users/wangbin/Desktop/openglEarthSatellite/res/images/right.png",                         "/Users/wangbin/Desktop/openglEarthSatellite/res/images/front.png", "/Users/wangbin/Desktop/openglEarthSatellite/res/images/back.png");


    // Render Loop
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        timeSinceLastToggle += deltaTime;
        if (animation)
            frameToggled += deltaTime;


        keyboardInput(window, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // view / projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        skyBox.setUniforms(projection, glm::mat4(glm::mat3(view)));
        skyBox.Draw();

        // Render the sun object
        sunShader.use();
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);
        
        model = glm::translate(model, sunPos); // Center it (kinda)
        sunShader.setMat4("model", model);
        Sun.Draw(sunShader);


        planetShader.use();

        // Set the lighting
        planetShader.setVec3("viewPos", camera.Position);
        planetShader.setFloat("material.shininess", 32.0f);

        planetShader.setVec3("light.position", sunPos);
        planetShader.setVec3("light.ambient", 0.25f, 0.25f, 0.25f);
        planetShader.setVec3("light.diffuse", 1.8f, 1.8f, 1.8f);
        planetShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[0].constant", 1.0f);
        planetShader.setFloat("pointLights[0].linear", 0.045);
        planetShader.setFloat("pointLights[0].quadratic", 0.0075);


        // Render the Earth object
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

        
        // Orbit around the sun
        earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);
        model = glm::translate(model, earthPos);
        // Rotate around itself
        model = glm::rotate(model, frameToggled * 1.5f * glm::radians(-50.0f), glm::vec3(0.1f, 1.0f, 0.0f));

        planetShader.setMat4("model", model);
        Earth.Draw(planetShader);

        // Draw a circle showing the earth's orbit around the sun
        EarthOrbitCircle.setUniforms(projection, view);
        EarthOrbitCircle.scale(glm::vec3(0.1f, 0.1f, 0.1f));
        EarthOrbitCircle.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        EarthOrbitCircle.Draw();


        // Render the Moon object
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        // Orbit around the earth
        glm::vec3 moonPos = earthPos + glm::vec3(0.0f, sin(frameToggled) * moonOrbitRadius , cos(frameToggled) * moonOrbitRadius);
        model = glm::translate(model, moonPos);
        planetShader.use();
        planetShader.setMat4("model", model);
        Moon.Draw(planetShader);

        // Draw a circle showing the moon's orbit around the earth
        MoonOrbitCircle.setUniforms(projection, view);
        MoonOrbitCircle.scale(glm::vec3(0.1f, 0.1f, 0.1f));
        MoonOrbitCircle.translate(earthPos);
        MoonOrbitCircle.rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        MoonOrbitCircle.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// Handles user keyboard input. Supposed to be used every frame, so deltaTime can be calculated appropriately.
void keyboardInput(GLFWwindow * window, float deltaTime)
{
    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Upwards Rotation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Orbit(UP, cameraOrbitRadius, rotateAngle);

    // Downwards Rotation
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Orbit(DOWN, cameraOrbitRadius, rotateAngle);

    // Rightwards Rotation
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Orbit(RIGHT, cameraOrbitRadius, rotateAngle);

    // Leftwards Rotation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Orbit(LEFT, cameraOrbitRadius, rotateAngle);


    // Pause / Start
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (timeSinceLastToggle > 0.2)
        {
            animation = !animation;
            timeSinceLastToggle = 0.0f;
        }
    }
        
}

// Handles mouse scroll wheel. Supposed to be used as the glfw scroll callback.
void scrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.Zoom(yoffset);
}
