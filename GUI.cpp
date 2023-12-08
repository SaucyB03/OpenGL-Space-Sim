//
// Created by brado on 10/20/2023.
//

#include "GUI.h"


void GUI::updateView() {
    inView = !inView;
    for(EnterValue* ev : entrVals){
        ev->changeView(inView);
    }

    for(Button* b : btns){
        b->changeView(inView);
    }
}

void GUI::resetGUI() {
    for(EnterValue* ev : entrVals){
        ev->reset();
    }

    for(Button* b : btns){
        b->reset();
    }
}



Object* GUI::onSubmit(string& earthFile, string& cometFile, string& sunFile, string& uvFile) {


//    Initialize vars for planet:
    glm::vec3 position {entrVals[0]->onSubmit<float>(),entrVals[1]->onSubmit<float>() + 3.0,entrVals[2]->onSubmit<float>()};
    float radius = entrVals[6]->onSubmit<float>();
    glm::vec3 velocity {entrVals[3]->onSubmit<float>(),entrVals[4]->onSubmit<float>(),entrVals[5]->onSubmit<float>()};
    float mass = entrVals[7]->onSubmit<float>() *  pow(10,20);
    glm::vec3 spin{entrVals[8]->onSubmit<float>(),entrVals[9]->onSubmit<float>(),entrVals[10]->onSubmit<float>()};
    string textureFilename;
//    Assign entered texture, if multiple it picks the first entered, and if none picked then uv map is displayed
    if(btns[0]->getVal()){
        textureFilename = earthFile;
    }else if(btns[1]->getVal()){
        textureFilename = cometFile;
    }else if(btns[2]->getVal()){
        textureFilename = sunFile;
    }else{
        textureFilename = uvFile;
    }

    glm::vec2 uvScale{entrVals[11]->onSubmit<float>(),entrVals[12]->onSubmit<float>()};

    bool isDynamic = true;

//    Reset all inputs values:
    resetGUI();
//    create planet:
    return new Object(position, {radius,radius,radius}, velocity, mass, spin, static_cast<Shape>(Circle), textureFilename, uvScale, true, scrWidth, scrHeight);

}


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
//    Position Inputs:
    entrVals.push_back(new EnterValue({-0.4, 0.35}, {0.25,0.1}, guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({-0.25, 0.35}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({-0.10, 0.35}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
//    Initial Velocity Inputs:
    entrVals.push_back(new EnterValue({-0.4, 0.15}, {0.25,0.1}, guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({-0.25, 0.15}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({-0.10, 0.15}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
//    Radius Input:
    entrVals.push_back(new EnterValue({-0.325, -0.05}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
//    Mass Input:
    entrVals.push_back(new EnterValue({-0.175, -0.05}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
//    Spin Input:
    entrVals.push_back(new EnterValue({-0.4, -0.25}, {0.25,0.1}, guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({-0.25, -0.25}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({-0.10, -0.25}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.0, texCol, true, scrWidth,scrHeight));

//  Texture options:
    btns.push_back(new Button({0.4,0.3}, {0.05,0.05}, guiCol, selGuiCol, RoundedSquare, 0.01, "Earth",{-0.05,0.0}, texScale, texCol, new Alignment[2]{Right,Center}, true, true, true, scrWidth,scrHeight));
    btns.push_back(new Button({0.4,0.2}, {0.05,0.05}, guiCol, selGuiCol, RoundedSquare, 0.01, "Comet",{-0.05,0.0}, texScale, texCol, new Alignment[2]{Right,Center}, true, false, true, scrWidth,scrHeight));
    btns.push_back(new Button({0.4,0.1}, {0.05,0.05}, guiCol, selGuiCol, RoundedSquare, 0.01, "Sun",{-0.05,0.0}, texScale, texCol, new Alignment[2]{Right,Center}, true, false, true, scrWidth,scrHeight));

//    Texture UV Scale:
    entrVals.push_back(new EnterValue({0.175, -0.125}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.181818, texCol, true, scrWidth,scrHeight));
    entrVals.push_back(new EnterValue({0.325, -0.125}, {0.25,0.1},guiCol, selGuiCol, RoundedSquare, 0.025, 0.333333, texCol, true, scrWidth,scrHeight));

//    Dynamic Toggle:
    btns.push_back(new Button({0.25,-0.25}, {0.05,0.05}, guiCol, selGuiCol, RoundedSquare, 0.01, "Dynamic",{-0.02,0.0}, texScale, texCol, new Alignment[2]{Right,Center}, true, false, true, scrWidth,scrHeight));


//    Submit button:
    btns.push_back(new Button({0.0,-0.4}, {0.25,0.1}, guiCol, selGuiCol, RoundedSquare, 0.025, "Submit!",{0.0,0.0}, texScale, texCol, new Alignment[2]{Center,Center}, false, false, true, scrWidth,scrHeight));
}

// Render call, displays each of the GUI elements
void GUI::RenderGUI(Shader* shader, Shader* textShader) {
    if(inView){
        for(EnterValue* ev : entrVals){
            ev->display(shader,textShader);
        }

        for(Button* b : btns){
            b->display(shader, textShader);
        }

//         Text:
        text.RenderText(textShader,"Position (km * 10^6): ", {scrWidth*0.14, scrHeight * 0.97}, texScale, texCol, new Alignment[2]{Center, Top});
        text.RenderText(textShader,"x                                  y                                  z", {scrWidth*0.1, scrHeight * 0.92}, texScale, texCol, new Alignment[2]{Center, Top});
        text.RenderText(textShader,"Initial Velocity (km/s): ", {scrWidth*0.15, scrHeight * 0.775}, texScale, texCol, new Alignment[2]{Center, Top});
        text.RenderText(textShader,"x                                  y                                  z", {scrWidth*0.1, scrHeight * 0.725}, texScale, texCol, new Alignment[2]{Center, Top});
        text.RenderText(textShader,"Diameter (km * 10^6):           Mass (kg * 10^20):", {scrWidth*0.27, scrHeight*0.525}, texScale, texCol, new Alignment[2]{Center, Top});

        text.RenderText(textShader,"Spin: (rad/s)", {scrWidth/10, scrHeight*0.375}, texScale, texCol, new Alignment[2]{Center, Top});
        text.RenderText(textShader,"x                                  y                                  z", {scrWidth*0.1, scrHeight*0.32}, texScale, texCol, new Alignment[2]{Center, Top});

        text.RenderText(textShader,"Textures: ", {scrWidth/1.4, scrHeight* 0.7}, texScale, texCol, new Alignment[2]{Center, Top});

        text.RenderText(textShader,"UV Scale (Earth Recommended  x:0.1818, y:0.3333): ", {scrWidth/1.25, scrHeight/2}, texScale, texCol, new Alignment[2]{Center, Top});
        text.RenderText(textShader,"x                                  y", {scrWidth*0.675, scrHeight*0.45}, texScale, texCol, new Alignment[2]{Center, Top});

    }
}

//Determines if the click event was on a GUI element
bool GUI::click_callback(glm::vec2 mousePos) {
    selectedIndex = -1;
//    Checks the entervalue elements for clicks, if clicked then it becomes selected
    for(int evInd = 0; evInd < entrVals.size(); ++evInd){
        bool isOver = entrVals[evInd]->checkSelect(mousePos);

        if(isOver){
            selectedIndex = evInd;
        }
    }

//    Checks button input if the last one is clicked (submit button) then we know to submit the form
    for(int bInd = 0; bInd < btns.size(); ++bInd){
        if(bInd == btns.size() - 1) {
            if(btns[bInd]->clickOnButton(mousePos)){
                inView = false;
                return true;
            }
        }else{
            btns[bInd]->clickOnButton(mousePos);
        }
    }
    return false;
}

void GUI::key_callback(int key) {
    if(((key >= GLFW_KEY_A && key <= GLFW_KEY_Z) || (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || key == GLFW_KEY_PERIOD || key == GLFW_KEY_MINUS) && selectedIndex != -1 and !kPress) {
        char character = static_cast<char>(key);
        entrVals[selectedIndex]->addToString(character);

    }else if (key == GLFW_KEY_BACKSPACE && selectedIndex != -1){
        entrVals[selectedIndex]->deleteChar();
    }else if(key == GLFW_KEY_ENTER && selectedIndex != -1){
        if(entrVals[selectedIndex]->enter()){
            selectedIndex = -1;
        }
    }else if(key == GLFW_KEY_RIGHT_ALT){
        updateView();
    }
}

//Checks the input every frame and calls the repsective functions for handling
bool GUI::checkInput(GLFWwindow* window) {
    int key;
    glfwGetKey(window, key);
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !mPress) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        mPress = !mPress;
        return click_callback({x, y});
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
    return false;

}

bool GUI::getView() {
    return inView;
}

