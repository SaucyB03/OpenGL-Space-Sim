//
// Created by brado on 10/20/2023.
//

#ifndef PLAYGROUND_GUI_H
#define PLAYGROUND_GUI_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Text.h"
#include "Shader.h"
#include "GUIElements/Button.h"
#include "GUIElements/EnterValue.h"

#include "vector"
#include "string"
#include "iostream"

// Per element we need:
// location
// scale
// color(s)
// input handling
// a defined output
// Labels


class GUI {
private:
    int scrWidth, scrHeight;

    int selectedIndex = -1;

    vector<EnterValue*> entrVals;
    vector<Button*> btns;
    int currentMenuIndex = 0;

    bool mPress = false;
    int kPress = 0;

    void click_callback(glm::vec2 mousePos);
    void key_callback(int key);

public:
    GUI(int ScreenWidth, int ScreenHeight);
    ~GUI();
    void initGUI();
    void RenderGUI(Shader* shader, Shader* textShader);

    void checkInput(GLFWwindow* window);

};



#endif //PLAYGROUND_GUI_H
