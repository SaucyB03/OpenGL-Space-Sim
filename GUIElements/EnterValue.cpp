//
// Created by brado on 11/23/2023.
//

#include "EnterValue.h"


void EnterValue::updateString(){
    if(!isnan(fvalue)){
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << fvalue;
        strText = stream.str();
    }else if(!empty(svalue)){
        strText = svalue;
    }else{
        strText = "";
    }
}

EnterValue::EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, glm::vec3 selCol, guiShape guiShape, float rsRadius, float initialValue, glm::vec3 texCol, bool inView, int scrWidth, int scrHeight) : guiObject(position, scale, rsRadius, color, guiShape, scrWidth, scrHeight){
    this->fvalue = initialValue;
    this->initialValF = initialValue;
    this->inView = inView;
    this->origCol = color;
    this->selCol = selCol;
    this->texCol = texCol;
    this->type = FLOAT;
    this->text = new Text();
    // Initialize the default value to the string
    updateString();
}

EnterValue::EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, glm::vec3 selCol, guiShape guiShape, float rsRadius, string initialValue, glm::vec3 texCol, bool inView, int scrWidth, int scrHeight) : guiObject(position, scale, rsRadius, color, guiShape, scrWidth, scrHeight){
    this->svalue = initialValue;
    this->initialValS = initialValue;
    this->inView = inView;
    this->origCol = color;
    this->selCol = selCol;
    this->texCol = texCol;
    this->type = STRING;
    this->text = new Text();
    // Initialize the default value to the string
    updateString();
}

void EnterValue::display(Shader* shader, Shader* texShader) {
    if(inView) {

        if(isSelected){
            color = selCol;
        }else{
            color = origCol;
        }

        this->guiObject::display(shader);
//        updateString();

        if (strText != ""){
            float texScale = 0.0;

            if(scale.x >= scale.y){
                texScale = scale.x * 3;
            }else{
                texScale = scale.y * 3;
            }
            glm::vec2 textPosPix = convertScSpToPix({position.x - scale.x/5, position.y});
            text->RenderText(texShader, strText, textPosPix, texScale, texCol, texAlign);
        }
    }
}

void EnterValue::changeView(bool inView){
    this->inView = inView;
}


void EnterValue::addToString(char c) {
    strText += c;
}

void EnterValue::deleteChar() {
    if(!strText.empty()){
        strText.pop_back();
    }
}

bool EnterValue::enter() {
    if(type == FLOAT){
        try{
            fvalue = std::stof(strText);
            updateString();
        }catch (const std::invalid_argument& e){
            cout<< "ERROR Invalid Argument in EnterValues: Expecting value float, non float entered. Try again"<<endl;
            return false;
        }
    }else{
        svalue = strText;
    }
    isSelected = false;
    return true;
}

void EnterValue::reset() {
    if(type == FLOAT){
        fvalue = initialValF;
    }else{
        svalue = initialValS;
    }
    updateString();
}


bool EnterValue::checkSelect(glm::vec2 mousePos){
// Convert GLFW input coords to OpenGL Screen Space Coords
    glm::vec2 mPosPix = convertPixToScSp(mousePos);

//    Determine if the button is on the screen if and we are clicking its bounding box:
//    Also marks if this guiShape is the selected for key input
    if(inView){
        if(mPosPix.x >= position.x - scale.x/2 && mPosPix.x <= position.x + scale.x/2 && mPosPix.y >= position.y - scale.y/2 && mPosPix.y <= position.y + scale.y/2) {
            isSelected = true;
            return true;
        }else{
            isSelected = false;
        }
    }else{
        isSelected = false;
    }
    return false;
}

bool EnterValue::selected() {
    return isSelected;
}

template<typename sbmt>
sbmt EnterValue::onSubmit() {
    return nullptr;
}



template<>
float EnterValue::onSubmit() {
    return fvalue;
}

template<>
string EnterValue::onSubmit() {
    return svalue;
}



