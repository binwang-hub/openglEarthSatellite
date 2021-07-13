#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const glm::vec3 POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
        
    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera Options
    float MovementSpeed;
    float MouseSensitivity;
    float zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = POSITION , glm::vec3 up = WORLDUP, float yaw = YAW, float pitch = PITCH);

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = YAW, float pitch = PITCH);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Moves the camera towards the direction given. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Move(Camera_Movement direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction
    void RotateMouse(float xoffset, float yoffset, GLboolean constrainPitch = true);
    
    // Rotates camera towards specific direction by angle. **Angle is NOT in radians**.
    void RotateRad(Camera_Movement direction, float angle, GLboolean constrainPitch = true);

        // Orbits around lookat as center. x, y axis
        // Idea:
        // 1. Move to center
        // 2. Rotate how you like (with angles (set value each time))
        // 3. Update Front, Right
        // 4. Move -Front * radius
    void Orbit(Camera_Movement direction, float radius, float angle);
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void Zoom(float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

#endif
