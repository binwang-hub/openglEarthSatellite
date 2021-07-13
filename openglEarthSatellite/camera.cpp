//
//  camera.cpp
//  openglEarthSatellite
//
//  Created by Wang Bin on 2021/7/10.
//

#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: Front(FRONT), Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, (Position + Front), Up);
}

void Camera::Move(Camera_Movement direction, float deltaTime)//processkeyboard
{
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Front * velocity;

    if (direction == BACKWARD)
        Position -= Front * velocity;

    if (direction == LEFT)
        Position -= Right * velocity;

    if (direction == RIGHT)
        Position += Right * velocity;

    if (direction == UP)
        Position += Up * velocity;

    if (direction == DOWN)
        Position -= Up * velocity;

}

void Camera::RotateMouse(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::RotateRad(Camera_Movement direction, float angle, GLboolean constrainPitch)
{
    switch(direction)
    {
        case UP:
            Pitch -= angle;
            break;

        case DOWN:
            Pitch += angle;
            break;

        case LEFT:
            Yaw += angle;
            break;

        case RIGHT:
            Yaw -= angle;
            break;

        default: break;
    }

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::Orbit(Camera_Movement direction, float radius, float angle)
{
    Position += Front * radius;

    RotateRad(direction, angle);
    
    Position -= Front * radius;
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::Zoom(float yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;

    if (zoom <= 1.0f)
        zoom = 1.0f;

    if (zoom >= 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
