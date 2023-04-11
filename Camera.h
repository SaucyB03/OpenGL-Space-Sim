#ifndef OE43DTESTER_CAMERA_H
#define OE43DTESTER_CAMERA_H

#include "Object.h"

class Camera{
private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 view;
    glm::vec3 angles = {-90,0.0,0.0};
    float speed = 5.0;

    bool firstMouse = true;
    float lastX = 0.0, lastY = 0.0;

public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);
    ~Camera();

    void applyView(Shader* shader);
    void move(glm::vec3 move, float deltaTime);
    void lookAround();

    void mouseCallback(GLFWwindow *window, double x, double y);

    template<typename T>
    void lookAt(T location);

};



#endif //OE43DTESTER_CAMERA_H
