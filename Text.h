//
// Created by brado on 10/20/2023.
//

#ifndef PLAYGROUND_TEXT_H
#define PLAYGROUND_TEXT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "map"
#include "filesystem"
#include "vector"

enum Alignment{
    Center,
    Left,
    Right,
    Top,
    Bottom
};

class Text {
private:
    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    string text;
    glm::vec2 position;
    float scale;
    glm::vec3 color;

    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;


    bool initText();
public:
    Text();
    void RenderText(Shader* shader, std::string text, glm::vec2 loc, float scale, glm::vec3 color, Alignment alignment[2]);

};

#endif //PLAYGROUND_TEXT_H
