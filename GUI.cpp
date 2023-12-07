//
// Created by brado on 10/20/2023.
//

#include "GUI.h"

GUI::GUI(int ScreenWidth, int ScreenHeight) {
    this->scrWidth = ScreenWidth;
    this->scrHeight = ScreenHeight;
    initGUI();
}
GUI::~GUI() {
    for (EnterValue* entry : entrVals) {
        delete entry;
    }
}


//Creates all the GUI elements comprising the display
void GUI::initGUI() {
    entrVals.push_back(new EnterValue({-0.4, 0.4}, {0.25,0.1}, {0.1,0.1,0.1}, RoundedSquare, 0.025, 0.0, {1.0,1.0,1.0}, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({-0.25, 0.4}, {0.25,0.1},{0.1,0.1,0.1}, RoundedSquare, 0.025, 0.0, {1.0,1.0,1.0}, true, scrWidth,scrHeight));
    btns.push_back(new Button({0,0}, {0.25,0.1}, {0.1,0.1,0.1}, RoundedSquare, 0.025, "Button!",{0.0,0.0}, 0.5, {1.0,1.0,1.0}, new Alignment[2]{Center,Center}, true, false, true, scrWidth,scrHeight));
    btns.push_back(new Button({-0.25,0}, {0.25,0.1}, {0.1,0.1,0.1}, RoundedSquare, 0.025, "Submit!",{0.0,0.0}, 0.5, {1.0,1.0,1.0}, new Alignment[2]{Center,Center}, false, false, true, scrWidth,scrHeight));
}

// Render call, displays each of the GUI elements
void GUI::RenderGUI(Shader* shader, Shader* textShader) {
    for(EnterValue* ev : entrVals){
        ev->display(shader,textShader);
    }

    for(Button* b : btns){
        b->display(shader, textShader);
    }
}

//Determines if the click event was on a GUI element
void GUI::click_callback(glm::vec2 mousePos) {
    selectedIndex = -1;
//    Checks the entervalue elements for clicks, if clicked then it becomes selected
    for(int evInd = 0; evInd < entrVals.size(); ++evInd){
        bool isOver = entrVals[evInd]->checkSelect(mousePos);
        cout << boolalpha << isOver << endl;

        if(isOver){
            selectedIndex = evInd;
        }
    }

//    Checks button input if the last one is clicked (submit button) then we know to submit the form
    for(int bInd = 0; bInd < btns.size(); ++bInd){
        if(bInd == btns.size() - 1) {
            if(btns[bInd]->clickOnButton(mousePos)){
                cout << "Run Submit!" << endl;
            }
        }else{
            btns[bInd]->clickOnButton(mousePos);
            cout << "Toggle!" << endl;
        }
    }
}

void GUI::key_callback(int key) {
    if(((key >= GLFW_KEY_A && key <= GLFW_KEY_Z) || (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || key == GLFW_KEY_PERIOD) && selectedIndex != -1 and !kPress) {
        char character = static_cast<char>(key);
        entrVals[selectedIndex]->addToString(character);

    }else if (key == GLFW_KEY_BACKSPACE && selectedIndex != -1){
        entrVals[selectedIndex]->deleteChar();
    }else if(key == GLFW_KEY_ENTER && selectedIndex != -1){
        if(entrVals[selectedIndex]->enter()){
            selectedIndex = -1;
        }
    }
}

//Checks the input every frame and calls the repsective functions for handling
void GUI::checkInput(GLFWwindow* window) {
    int key;
    glfwGetKey(window, key);
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !mPress) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        click_callback({x, y});
        mPress = !mPress;
    }else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && mPress){
        mPress = !mPress;
    }else{
        // Checks through all possible key presses to determine if a key was pressed:
        for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {

            if (glfwGetKey(window, key) == GLFW_PRESS && kPress != key) {
                key_callback(key);
                kPress = key;
                break;
            }else if(kPress == key && glfwGetKey(window, key) != GLFW_PRESS){
                kPress = 0;
                break;
            }
        }
    }


}

