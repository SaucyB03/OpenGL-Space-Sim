#include "Camera.h"
#include "glm/glm.hpp"

//Camera Constructor, initializes its variables
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
    this->cameraPos = position;
    this->cameraFront = front;
    this->cameraUp = up;

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

Camera::~Camera() {

}

//Passes the view and its position to the shader
void Camera::applyView(Shader* shader) {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader->setUniformMat4("view", view);
    shader->setVec3("viewPos", cameraPos);
}

//move camera by the entered amount (note: moves locally, so rotating camera changes its axis of movemement)
void Camera::move(glm::vec3 move, float deltaTime) {

    cameraPos.x +=  (move.z * cos(glm::radians(angles.x)) - move.x * sin(glm::radians(angles.x))) * speed * deltaTime;
    cameraPos.z +=  (move.z * sin(glm::radians(angles.x)) + move.x * cos(glm::radians(angles.x))) * speed * deltaTime;

    cameraPos.y += move.y * speed * deltaTime;
}

void Camera::lookAround() {

}

//Handling mouse input:
void Camera::mouseCallback(GLFWwindow *window, double x, double y) {
    float xpos = static_cast<float>(x);
    float ypos = static_cast<float>(y);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    //update angles based off change in mouse
    angles.x   += xoffset;
    angles.y += yoffset;

    if(angles.y > 89.0f)
        angles.y = 89.0f;
    if(angles.y < -89.0f)
        angles.y = -89.0f;

    //Calculate direction camera is facing based off angles
    glm::vec3 direction;
    direction.x = cos(glm::radians(angles.x)) * cos(glm::radians(angles.y));
    direction.y = sin(glm::radians(angles.y));
    direction.z = sin(glm::radians(angles.x)) * cos(glm::radians(angles.y));
    cameraFront = glm::normalize(direction);
}
