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

    Alignment texAlign[2] = {Left, Center};

    string strText;
    glm::vec3 texCol;
    Text* text;
    bool isSelected;
    bool inView;
    glm::vec2 scrPixelScale;
    EVType type;

    void updateString();

public:

    EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, guiShape guiShape, float rsRadius, float initialValue, glm::vec3 texCol, bool inView, int scrWidth, int scrHeight);
    EnterValue(glm::vec2 position, glm::vec2 scale, glm::vec3 color, guiShape guiShape, float rsRadius, string initialValue, glm::vec3 texCol, bool inView,  int scrWidth, int scrHeight);

    void display(Shader* shader, Shader* texShader);

    void changeView(bool inView);

    void addToString(char c);
    void deleteChar();
    bool enter();

    bool checkSelect(glm::vec2 mousePos);
    bool selected();



    // Creating templates so that either float or string values can be entered
    template<typename updt>
    void updateText(updt value);

    template<typename sbmt>
    sbmt onSubmit();
};


#endif //PLAYGROUND_ENTERVALUE_H
