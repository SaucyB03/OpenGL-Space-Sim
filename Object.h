//Parent Class of all onscreen objects

#ifndef M3OE_BKSOUCY_OBJECT_H
#define M3OE_BKSOUCY_OBJECT_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "stb-master/stb_image.h"
#include <vector>

#include "Shader.h"

const double APPARENT_GRAVITY = 981;

enum Shape{
    Cube,
    Sphere
};

class Object {
private:
    int scWidth;
    int scHeight;

    unsigned int texture1;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 velocity;
    float mass;
    glm::vec3 color;
    float angle = 0.0f;
    float uvScale;


    vector<float> vertices;
    vector<int> indices;
    vector<float> textCords;
    vector<float> normals;

    bool dynamic;
    //Add necessary buffers
    unsigned int va, vb, eb;

    /* assignBuffandArr
     * Assigned the Buffers and Vertex arrays for this object as well as store the information in a
     * Element array Buffer, which allows us to put 2 triangles together and have OpenGL know what to do
     */
    void assignBuffandArr();

    void generateSphere();
    void generateCube();


    void subdivideSphere(float radius);
    glm::vec3 findMidpoint(int vertex1, int vertex2, float radius, string type);
    void calculateNormals();

    void addVerts(vector<float> &vector, glm::vec3 info);
    void addIndices(vector<int> &vector, glm::vec3 info);
public:
    /* Object Constructor
     * Holds info about: position, scale, velocity, color, if its dynamic / static, and the screen size
     */
    Object(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, float mass, glm::vec3 color, Shape shape, float uvScale, bool dynamic, int scWidth, int scHeight);
    //Destructor: deletes the created buffers
    ~Object();

    /* Basic move
     * if there's a velocity on the object then it updates it from the last frame
     */
    void move(float deltaTime);
    /*Display
     * Applys any transformations the object has had and displays the object
     */
    void display(Shader* shader);

    /*getPostion
     * returns the objects position as a vec2
     */
    glm::vec3 getPosition();
    /*getVelocity
     * returns the objects velocity as a vec2
     */
    glm::vec3 getVelocity();
    /*getScale
     * returns the objects scale as a vec2
     */
    glm::vec3 getScale();

    /* setVelocity
     * sets the velocity to the inputted vec2
     */
    void setVelocity(glm::vec3 newVelocity);
    void setAngle(float add);
};


#endif //M3OE_BKSOUCY_OBJECT_H
