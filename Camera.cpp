#include "Camera.h"
#include "glm/glm.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
    this->cameraPos = position;
    this->cameraFront = front;
    this->cameraUp = up;

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

Camera::~Camera() {

}

void Camera::applyView(Shader* shader) {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader->setUniformMat4("view", view);
    shader->setVec3("viewPos", cameraPos);
}

void Camera::move(glm::vec3 move, float deltaTime) {

    cameraPos.x +=  (move.z * cos(glm::radians(angles.x)) - move.x * sin(glm::radians(angles.x))) * speed * deltaTime;
    cameraPos.z +=  (move.z * sin(glm::radians(angles.x)) + move.x * cos(glm::radians(angles.x))) * speed * deltaTime;

    cameraPos.y += move.y * speed * deltaTime;
}

void Camera::lookAround() {

}


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

    angles.x   += xoffset;
    angles.y += yoffset;

    if(angles.y > 89.0f)
        angles.y = 89.0f;
    if(angles.y < -89.0f)
        angles.y = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(angles.x)) * cos(glm::radians(angles.y));
    direction.y = sin(glm::radians(angles.y));
    direction.z = sin(glm::radians(angles.x)) * cos(glm::radians(angles.y));
    cameraFront = glm::normalize(direction);
}

template<typename T>
void Camera::lookAt(T location) {

}

template<>
void Camera::lookAt<Object&>(Object& object){
    //cameraFront = object.getPosition()
}

template<>
void Camera::lookAt<glm::vec3>(glm::vec3 location){

}
