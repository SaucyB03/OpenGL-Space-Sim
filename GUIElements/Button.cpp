//
// Created by brado on 10/24/2023.
//

#include "Button.h"


Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius, string text, glm::vec2 localTexLoc, float texScale, glm::vec3 texCol, Alignment* texAlign, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight) : Object(position, scale, rsRadius, color, shape, scrWidth,scrHeight) {
    this->textStr = text;
    this->texLoc = localTexLoc;
    this->texScale = texScale;
    this->texCol = texCol;
    this->texAlign[0] = texAlign[0];
    this->texAlign[1] = texAlign[1];
    this->toggleable = toggleable;
    this->On = defaultVal;
    this->inView = inView;
    this->scrPixelScale = {scrWidth, scrHeight};
    this->physicalText = new Text();
}

Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius, string text, glm::vec2 localTexLoc, float texScale, glm::vec3 texCol, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight)
        : Button(position, scale, color, shape, rsRadius, text, localTexLoc, texScale, texCol, new Alignment[2]{Center,Bottom}, toggleable, defaultVal, inView, scrWidth, scrHeight){

}

Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius, string text, float texScale, glm::vec3 texCol, Alignment* texAlign, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight)
        : Button(position, scale, color, shape, rsRadius, text, {0,0}, texScale, texCol, texAlign, toggleable, defaultVal, inView,  scrWidth, scrHeight){

}

Button::Button(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius, bool toggleable, bool defaultVal, bool inView, int scrWidth, int scrHeight)
        : Button(position, scale, color, shape, rsRadius, "", {0,0}, 0, {0,0,0}, new Alignment[2]{Center,Bottom}, toggleable, defaultVal, scrWidth, scrHeight){

}

Button::~Button() {
    this->Object::~Object();
}

void Button::display(Shader* objShader, Shader* texShader) {
    if(inView) {
        this->Object::display(objShader);

        if (textStr != "") {
            glm::vec2 textPosPix = convertScSpToPix(position + texLoc);
            physicalText->RenderText(texShader, this->textStr, textPosPix, this->texScale, this->texCol,
                                     this->texAlign);
        }
    }
}

bool Button::clickOnButton(glm::vec2 clickPos) {
    if(inView){
        glm::vec2 covMPos = convertPixToScSp(clickPos);

        if(covMPos.x >= position.x - scale.x/2
        && covMPos.x <= position.x + scale.x/2
        && covMPos.y >= position.y - scale.y/2
        && covMPos.y <= position.y + scale.y/2){

            cout<< "click"<<endl;
            if(toggleable){
                On = !On;
                glm::vec3 tempCol = color;
                color = toggleCol;
                toggleCol = tempCol;
            }else{
                inView = false;
            }
            return true;
        }
    }
    return false;
}

bool Button::isPressed() {
    return pressed;
}

void Button::setPressed(bool press) {
    this->pressed = press;
}

bool Button::isToggleable() {
    return toggleable;
}


