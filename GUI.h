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
#include "./Object.h"

#include "vector"
#include "string"
#include "iostream"
#include "filesystem"




class GUI {
private:
    int scrWidth, scrHeight;

    int selectedIndex = -1;
    const glm::vec3 rgb = {255.0,255.0,255.0};

    glm::vec3 guiCol = glm::vec3{53, 139, 48} / rgb; // Gui's base color
    glm::vec3 selGuiCol = glm::vec3{4, 71, 1} / rgb;
    glm::vec3 texCol = glm::vec3{143, 208, 140}/ rgb;
    float texScale = 0.75;

    vector<EnterValue*> entrVals;
    vector<Button*> btns;
    int currentMenuIndex = 0;

    bool mPress = false;
    int kPress = 0;
    bool inView = false;
    Text text;

    bool click_callback(glm::vec2 mousePos);
    void key_callback(int key);

    void updateView();
    void resetGUI();

public:
    GUI(int ScreenWidth, int ScreenHeight);
    ~GUI();
    void initGUI();
    void RenderGUI(Shader* shader, Shader* textShader);

    bool checkInput(GLFWwindow* window);

    Object* onSubmit(string& earthFile, string& cometFile, string& sunFile, string& uvFile);

    bool getView();
};



#endif //PLAYGROUND_GUI_H
