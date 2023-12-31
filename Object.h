//Parent Class of all onscreen objects

#ifndef M3OE_BKSOUCY_OBJECT_H
#define M3OE_BKSOUCY_OBJECT_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "stb-master/stb_image.h"

#include <vector>

#include "Shader.h"

//Shape Types:
enum Shape{
    Cube,
    Sphere
};

class Object {
private:
    //Constants:
    const int SPHERE_SUB = 3;
    const float UNIVERSAL_GRAVITY_CONSTANT = 6.673 * pow(10, -11);

    int scWidth;
    int scHeight;

    unsigned int texture1;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 velocity;
    float mass;
    glm::vec3 currentForce = glm::vec3(0.0f);
    glm::vec3 angle = {0.0,0.0,0.0};
    glm::vec3 spin;
    glm::vec2 uvScale;

    string textureFilename;


    //Object information for buffers
    vector<float> vertices;
    vector<int> indices;
    vector<float> textCords;
    vector<float> normals;

    bool dynamic;
    bool cubemap;
    //Add necessary buffers
    unsigned int va, vb, eb;

    /* assignBuffandArr
     * Assigned the Buffers and Vertex arrays for this object as well as store the information in a
     * Element array Buffer, which allows us to put 2 triangles together and have OpenGL know what to do
     */
    void assignBuffandArr();

    //Generates the respective 3D Shapes
    void generateSphere();
    void generateCube();

    //Subdivide Sphere: subdivides the sphere by the given global amount
    void subdivideSphere(float radius);
    //findMidpoint: calulcates the midpoint between 2 vertex's
    glm::vec3 findMidpoint(int vertex1, int vertex2, float radius, string type);
    //calculate normals: calculates the vertex normals of the shape
    void calculateNormals();

    //Simplifies adding vec3's to vectors<>
    void addVerts(vector<float> &vector, glm::vec3 info);
    void addIndices(vector<int> &vector, glm::vec3 info);
public:
    /* Object Constructor
     * Holds info about: position, scale, velocity, color, if its dynamic / static, and the screen size
     */
    Object(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, float mass, glm::vec3 spin, Shape shape, string textureFilename, glm::vec2 uvScale, bool dynamic, int scWidth, int scHeight);
    //Non-Dynamic/Non-interactive:
    Object(glm::vec3 position, glm::vec3 scale, Shape shape, string textureFilename, glm::vec2 uvScale, bool cubemap, int scWidth, int scHeight);
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

    void addForce(glm::vec3 otherObjPos, float otherObjMass);

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

    //Get mass: returns the mass of the object
    float getMass();

    /* setVelocity
     * sets the velocity to the inputted vec2
     */
    void setVelocity(glm::vec3 newVelocity);
    //setAngle changes the angle of the object (for spin)
    void setAngle(glm::vec3 add);
    //setFroce sets the force of the object
    void setForce(glm::vec3 force);
};


#endif //M3OE_BKSOUCY_OBJECT_H
