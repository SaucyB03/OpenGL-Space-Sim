//
// Created by brado on 10/24/2023.
//

#include "Button.h"


Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, glm::vec3 toggleCol, guiShape guiShape, float rsRadius, string text, glm::vec2 localTexLoc, float texScale, glm::vec3 texCol, Alignment* texAlign, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight) : guiObject(position, scale, rsRadius, color, guiShape, scrWidth,scrHeight) {
    this->textStr = text;
    this->texLoc = localTexLoc;
    this->texScale = texScale;
    this->texCol = texCol;
    this->texAlign[0] = texAlign[0];
    this->texAlign[1] = texAlign[1];
    this->toggleCol = toggleCol;
    this->origCol = color;
    this->toggleable = toggleable;
    this->On = defaultVal;
    this->initialVal = defaultVal;
    this->inView = inView;
    this->scrPixelScale = {scrWidth, scrHeight};
    this->physicalText = new Text();
}

Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, guiShape guiShape, float rsRadius, string text, glm::vec2 localTexLoc, float texScale, glm::vec3 texCol, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight)
        : Button(position, scale, color, {0,0,0}, guiShape, rsRadius, text, localTexLoc, texScale, texCol, new Alignment[2]{Center,Bottom}, toggleable, defaultVal, inView, scrWidth, scrHeight){

}

Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, guiShape guiShape, float rsRadius, string text, float texScale, glm::vec3 texCol, Alignment* texAlign, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight)
        : Button(position, scale, color,{0,0,0}, guiShape, rsRadius, text, {0,0}, texScale, texCol, texAlign, toggleable, defaultVal, inView,  scrWidth, scrHeight){

}

Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, guiShape guiShape, float rsRadius, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight)
        : Button(position, scale, color, {0,0,0}, guiShape, rsRadius, "", {0,0}, 0, {0,0,0}, new Alignment[2]{Center,Bottom}, toggleable, defaultVal, inView, scrWidth, scrHeight){

}

Button::~Button() {
    this->guiObject::~guiObject();
}

void Button::display(Shader* objShader, Shader* texShader) {
    if(inView) {
//        display shape
        if(On && toggleable){
            color = toggleCol;
        }else{
            color = origCol;
        }
        this->guiObject::display(objShader);

//        display text
        if (textStr != "") {
            glm::vec2 textPosPix = convertScSpToPix(position + texLoc);
            physicalText->RenderText(texShader, this->textStr, textPosPix, this->texScale, this->texCol,
                                     this->texAlign);
        }
    }
}

// Click handling, determines if the button has been clicked or not.
bool Button::clickOnButton(glm::vec2 clickPos) {
    if(inView){
//         get the screen space coords from glfw pixel coords
        glm::vec2 covMPos = convertPixToScSp(clickPos);

        if(covMPos.x >= position.x - scale.x/2
        && covMPos.x <= position.x + scale.x/2
        && covMPos.y >= position.y - scale.y/2
        && covMPos.y <= position.y + scale.y/2){

//            Determine what action to take if it is toggleable or not
            if(toggleable){
                On = !On;
            }else{
                inView = false;
            }
            return true;
        }
    }
    return false;
}

void Button::reset() {
    if(toggleable){
        On = initialVal;
    }
}

// Change whether itll be displayed to the screen or not
void Button::changeView(bool newView) {
    inView = newView;
}

// Returns if this button is toggleable or if its a submit button
bool Button::isToggleable() {
    return toggleable;
}

bool Button::getVal(){
    return On;
}




