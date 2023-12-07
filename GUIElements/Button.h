//
// Created by brado on 10/24/2023.
//

#ifndef PLAYGROUND_BUTTON_H
#define PLAYGROUND_BUTTON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Text.h"
#include "../object.h"



class Button : public Object {
private:
    string textStr;
    glm::vec2 texLoc;
    float texScale;
    glm::vec3 texCol;
    Alignment texAlign[2];
    Text* physicalText;
    bool toggleable;
    bool On;
    bool inView;
    bool pressed = false;

    glm::vec2 scrPixelScale;

    glm::vec3 toggleCol = {0,0,0};

public:
    Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius,
           string text, glm::vec2 localTexLoc, float texScale, glm::vec3 texCol, Alignment* texAlign,
           bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight);

    Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius,
           string text, glm::vec2 localTexLoc, float texScale, glm::vec3 texCol,
           bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight);

    Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius,
           string text, float texScale, glm::vec3 texCol, Alignment* texAlign,
           bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight);

    Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius,
           bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight);
    ~Button();

    void display(Shader* objShader, Shader* texShader);
    bool clickOnButton(glm::vec2 clickPos);

    bool isPressed();
    void setPressed(bool press);

    bool isToggleable();
};


#endif //PLAYGROUND_BUTTON_H
