//Parent Class of all onscreen guiObjects

#ifndef PLAYGROUND_guiObject_H
#define PLAYGROUND_guiObject_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

#include "../Shader.h"

//guiShape Types:
enum guiShape{
    Square,
    Circle,
    RoundedSquare
};

class guiObject {
private:
    int BASE_SUBDIVISIONS = 64;
    int RS_BS = 15;

    int scWidth;
    int scHeight;

    float rsRadius;

    //guiObject information for buffers
    vector<float> vertices;
    vector<int> indices;

    //Add necessary buffers
    unsigned int va, vb, eb;

    /* assignBuffandArr
     * Assigned the Buffers and Vertex arrays for this guiObject as well as store the information in a
     * Element array Buffer, which allows us to put 2 triangles together and have OpenGL know what to do
     */
    void assignBuffandArr();

    //Generates the respective 3D guiShapes
    void generateCircle();
    void generateSquare();
    void generateRoundedSquare();


    //Simplifies adding vec3's to vectors<>
    static void addVerts(vector<float> &vector, glm::vec3 info);
    static void addIndices(vector<int> &vector, glm::vec3 info);

protected:
    glm::vec2 convertPixToScSp(glm::vec2 pixCoord);
    glm::vec2 convertScSpToPix(glm::vec2 scSpCoord);

public:
    glm::vec2 position;
    glm::vec2 scale;
    glm::vec3 color;

    /* guiObject Constructor
     * Holds info about: position, scale, velocity, color, if its dynamic / static, and the screen size
     */
    guiObject(glm::vec2 position, glm::vec2 scale, float rsRadius, glm::vec3 color, guiShape guiShape, int scWidth, int scHeight);
    guiObject(glm::vec2 position, glm::vec2 scale, glm::vec3 color, guiShape guiShape, int scWidth, int scHeight);
    ~guiObject();

    /*Display
     * Applys any transformations the guiObject has had and displays the guiObject
     */
    void display(Shader* shader);



};


#endif //PLAYGROUND_guiObject_H
