//
// Created by brado on 11/23/2023.
//

#include "EnterValue.h"


EnterValue::EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius, float initialValue, glm::vec3 texCol, bool inView, int scrWidth, int scrHeight) : Object(position, scale, rsRadius, color, shape, scrWidth, scrHeight){
    this->fvalue = initialValue;
    this->inView = inView;
    this->texCol = texCol;
    this->type = FLOAT;
    this->text = new Text();
    // Initialize the default value to the string
    updateString();
}

EnterValue::EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, Shape shape, float rsRadius, string initialValue, glm::vec3 texCol, bool inView, int scrWidth, int scrHeight) : Object(position, scale, rsRadius, color, shape, scrWidth, scrHeight){
    this->svalue = initialValue;
    this->inView = inView;
    this->texCol = texCol;
    this->type = STRING;
    this->text = new Text();
    // Initialize the default value to the string
    updateString();
}

void EnterValue::display(Shader* shader, Shader* texShader) {
    if(inView) {
        this->Object::display(shader);

//        updateString();

        if (strText != ""){
            float texScale = 0.0;

            if(scale.x >= scale.y){
                texScale = scale.x * 2;
            }else{
                texScale = scale.y * 2;
            }
            glm::vec2 textPosPix = convertScSpToPix(position);
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


bool EnterValue::checkSelect(glm::vec2 mousePos){
// Convert GLFW input coords to OpenGL Screen Space Coords
    glm::vec2 mPosPix = convertPixToScSp(mousePos);

//    Determine if the button is on the screen if and we are clicking its bounding box:
//    Also marks if this shape is the selected for key input
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

template<typename updt>
void EnterValue::updateText(updt value) {
}

template<>
void EnterValue::updateText(string value) {
    svalue = value;
}

template<>
void EnterValue::updateText(float value) {
    fvalue = value;
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
