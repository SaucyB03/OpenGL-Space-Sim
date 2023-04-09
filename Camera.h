#ifndef OE43DTESTER_CAMERA_H
#define OE43DTESTER_CAMERA_H

#include "Object.h"

class Camera{
private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 view;
    float speed = 2.5;
public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);
    ~Camera();

    void applyView(Shader* shader);
    void move(glm::vec3 move, float deltaTime);
    void lookAround();

    template<typename T>
    void lookAt(T location);

};



#endif //OE43DTESTER_CAMERA_H
