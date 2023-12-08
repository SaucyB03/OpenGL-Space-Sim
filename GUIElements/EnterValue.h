//
// Created by brado on 11/23/2023.
//

#ifndef PLAYGROUND_ENTERVALUE_H
#define PLAYGROUND_ENTERVALUE_H

#include "guiObject.h"
#include "../Text.h"

#include <typeinfo>

enum EVType{
    FLOAT,
    STRING
};

class EnterValue : guiObject {
private:
    float fvalue;
    string svalue;
    glm::vec3 origCol;
    glm::vec3 selCol;
    float initialValF;
    string initialValS;

    Alignment texAlign[2] = {Left, Center};
    string strText;
    glm::vec3 texCol;
    Text* text;
    bool isSelected = false;
    bool inView;
    glm::vec2 scrPixelScale;
    EVType type;

// Updates the string based on the svalue if STRING type or fvalue if FLOAT type
// Allows for initialization of the string if given a float
    void updateString();

public:

    EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, glm::vec3 selCol, guiShape guiShape, float rsRadius, float initialValue, glm::vec3 texCol, bool inView, int scrWidth, int scrHeight);
    EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, glm::vec3 selCol, guiShape guiShape, float rsRadius, string initialValue, glm::vec3 texCol, bool inView,  int scrWidth, int scrHeight);

//    Displays the object
    void display(Shader* shader, Shader* texShader);

//    Change whether or not the object is visible on screen
    void changeView(bool inView);

//    Text input handling, adds / removes from the input box
    void addToString(char c);
    void deleteChar();

//    On enter pass the information into its respective variables fvalue for floats svalue for strings
//     That way its easy to return the input boxes given value
    bool enter();

//    Returns the input box to its original value, good for after a submission
    void reset();

//  Check mouse input and see if the object has been selected
    bool checkSelect(glm::vec2 mousePos);

//   Getter function returns if the input is currently selected
    bool selected();





    // Creating templates so that either float or string values can be entered
    template<typename sbmt>
    sbmt onSubmit();
};


#endif //PLAYGROUND_ENTERVALUE_H
