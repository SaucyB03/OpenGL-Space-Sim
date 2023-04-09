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
}

void Camera::move(glm::vec3 move, float deltaTime) {
    cameraPos += move * speed * deltaTime;
}

void Camera::lookAround() {

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