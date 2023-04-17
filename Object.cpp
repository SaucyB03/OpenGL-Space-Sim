//
// Created by brado on 3/5/2023.
//

#include "Object.h"


//Private////////////
/* assignBuffandArr
 * Assigned the Buffers and Vertex arrays for this object as well as store the information in a
 * Element array Buffer, which allows us to put 2 triangles together and have OpenGL know what to do
 */
void Object::assignBuffandArr(){
    //Assign Buffers and arrays
    glGenVertexArrays(1, &this->va);
    glGenBuffers(1, &this->vb);
    glGenBuffers(1, &this->eb);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(this->va);

    //If its dynamic tell OpenGl to draw it dynamically otherwise it will remain static
    if(dynamic){
        glBindBuffer(GL_ARRAY_BUFFER, this->vb);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + textCords.size() * sizeof(float) + normals.size() * sizeof(float), 0, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices.front());
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), textCords.size() * sizeof(float), &textCords.front());
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + textCords.size() * sizeof(float), normals.size() * sizeof(float), &normals.front());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_DYNAMIC_DRAW);
    }else {
        glBindBuffer(GL_ARRAY_BUFFER, this->vb);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + textCords.size() * sizeof(float) + normals.size() * sizeof(float), 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices.front());
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), textCords.size() * sizeof(float), &textCords.front());
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + textCords.size() * sizeof(float), normals.size() * sizeof(float), &normals.front());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);
    }

    //Assign vertex attribute pointer telling OpenGL how the array of verts is organized
    //i.e tell it if there is an offset or a stride it needs to be aware of
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertices.size() * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertices.size() * sizeof(float) + textCords.size() * sizeof(float)));
    glEnableVertexAttribArray(2);


    // Unbinds the buffer and vertex array now that we are done with them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Enables faceculling so faces facing the other direction from the camera arent being displayed
    glEnable(GL_CULL_FACE);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters`
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(textureFilename.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        if(textureFilename.substr(textureFilename.size()-3) == "jpg"){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }else{
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

    }
    else
    {
        std::cout << "ERROR: Failed to load texture of filename: '" << textureFilename << "'" << std::endl;
    }
    stbi_image_free(data);
}

void Object::generateSphere() {
    //Diagram of texture Unwrapping  +  Vertex# [spot in vertices arr]
    /*
     *        *   *   *   *   *               0[1],  0[3],  0[5],  0[7],  0[9]
     *       / \ / \ / \ / \ / \
     *      *---*---*---*---*---*          1[0],  2[2],  3[4],  4[6],  5[8],  1[10]
     *     / \ / \ / \ / \ / \ /
     *    *---*---*---*---*---*         6[11], 7[13], 8[15], 9[17], 10[19], 11[21]
     *     \ / \ / \ / \ / \ /
     *      *   *   *   *   *              12[12], 12[14], 12[16], 12[18], 12[20]
     */

    float radius = scale.x/2;


    //Algorithm that initializes the unsubdivided icosphere's vertices defines the triangles + textcords
    float phi, theta, LRot, x, y, z, tx = 0.5, ty = 1.0;
    int i, index = 0;
    for(i = 1; i <= 12; ++i) {
        phi = atan(0.5f);
        theta = 2 * M_PI / 5 * i - 2*LRot;

        //if bottom half of sphere make the y-cord(up/down) negative else positive
        if (i < 6) {
            //calculate vertex positions
            x = radius * cos(phi) * cos(theta);
            y = radius * sin(phi);
            z = radius * cos(phi) * sin(theta);

            //add triangle definitions and vertices
            addIndices(indices, {index + 1, index, index + 2});
            addIndices(indices, {index + 2, index, index + 13});
            addVerts(vertices, {x, y, z});
            addVerts(vertices, {0.0, scale.y / 2, 0.0});

            //define teh vertices textcords
            textCords.push_back(tx * uvScale.x);
            textCords.push_back(ty * uvScale.y);
            textCords.push_back((tx + 0.5f) * uvScale.x);
            textCords.push_back((ty - 1.0f) * uvScale.y);

            tx += 1.0f;

            index += 2;
        } else if (i > 6 && i < 12) {
            //calculate vertex positions
            x = radius * cos(-phi) * cos(theta);
            y = radius * sin(-phi);
            z = radius * cos(-phi) * sin(theta);

            //add triangle definitions and vertices
            addIndices(indices, {index - 1, index, index + 1});
            addIndices(indices, {index - 12, index - 1, index + 1});
            addVerts(vertices, {x, y, z});
            addVerts(vertices, {0.0, -scale.y / 2, 0.0});

            //define the vertices textcords
            textCords.push_back(tx * uvScale.x);
            textCords.push_back(ty * uvScale.y);
            textCords.push_back((tx + 0.5f) * uvScale.x);
            textCords.push_back((ty + 1.0f) * uvScale.y);

            tx += 1.0;

            index += 2;
        } else if(i == 12){
            //calculate vertex positions
            x = radius * cos(-phi) * cos(theta);
            y = radius * sin(-phi);
            z = radius * cos(-phi) * sin(theta);

            //add texture cords and vertices
            addVerts(vertices, {x, y, z});
            textCords.push_back(tx * uvScale.x);
            textCords.push_back(ty * uvScale.y);
        }else{
            //calculate vertex positions
            x = radius * cos(phi) * cos(theta);
            y = radius * sin(phi);
            z = radius * cos(phi) * sin(theta);

            //add texture cords and vertices
            addVerts(vertices, {x, y, z});
            textCords.push_back(tx * uvScale.x);
            textCords.push_back(ty * uvScale.y);

            tx = 0.0f;
            ty = 2.0f;

            LRot = M_PI/5;
            index += 2;
        }
    }

    subdivideSphere(radius);

    calculateNormals();
}

//Method to generate a cube
void Object::generateCube() {
    //Cube vertex index numbers
    //  6*------------*5
    //  /|           /|
    //2*------------*1|
    // | |          | |
    // |7*----------|-*4
    // |/           |/
    //3*------------*0

    //initialize vertices
    vertices = {
            scale.x/2,  -scale.y/2,    scale.z/2,  /* front bottom right */
            scale.x/2,  scale.y/2,    scale.z/2,  /* front top right */
            -scale.x/2,  scale.y/2,    scale.z/2,  /* front top left */
            -scale.x/2, -scale.y/2,   scale.z/2,   /* front bottom left */

            scale.x/2, -scale.y/2,  -scale.z/2,  /* front bottom right */
            scale.x/2, scale.y/2,  -scale.z/2,  /* front top right */
            -scale.x/2, scale.y/2,  -scale.z/2,  /* front top left */
            -scale.x/2, -scale.y/2,  -scale.z/2,   /* front bottom left */

            //Splitting vertices for unwrapping onto texture
            scale.x/2,  scale.y/2,    -scale.z/2,// vert #5 | 8

            scale.x/2,  scale.y/2,    scale.z/2, // vert #1 | 9
            scale.x/2,  scale.y/2,    scale.z/2, // vert #1 | 10

            -scale.x/2,  scale.y/2,    scale.z/2, // vert #2 | 11
            -scale.x/2,  scale.y/2,    scale.z/2, // vert #2 | 12

            -scale.x/2,  scale.y/2,   -scale.z/2  //vert #6 | 13

    };

    //Defining the triangles that make up the cube
    indices = {
            3, 1, 0,  // first Triangle
            1, 3, 2,   // second Triangle
            4, 6, 7,  //ect...
            6, 4, 5,
            7, 0, 4,
            0, 7, 3,
            6, 5, 9,
            9, 11, 6,
            4, 10, 8,
            10, 4, 0,
            13, 3, 7,
            3, 13, 12
    };

    //Diagram of Cube unwrapped:
    /*             2[12]     6[13]
     *             *---------*
     *             |         |
     *   2         |3        |7        6         2[11]
     *   *---------*---------*---------*---------*
     *   |         |         |         |         |
     *   |         |         |         |         |
     *   *---------*---------*---------*---------*
     *   1         |0        |4        5         1[9]
     *             |         |
     *             *---------*
     *             1[10]     5[8]
     */


    //Defining of texture cords:
    textCords = {
            -uvScale.x,2*uvScale.y,
            0.0,2*uvScale.y,
            0.0,uvScale.y,
            -uvScale.x,uvScale.y,

            -2*uvScale.x,2*uvScale.y,
            -3*uvScale.x,2*uvScale.y,
            -3*uvScale.x,uvScale.y,
            -2*uvScale.x,uvScale.y,

            -2*uvScale.x,3*uvScale.y,

            -4*uvScale.x,2*uvScale.y,
            -uvScale.x,3*uvScale.y,

            -4*uvScale.x,uvScale.y,
            -uvScale.x,0.0,

            -2*uvScale.x,0.0
    };

    calculateNormals();
}


//Subdivides the sphere by taking each triangle and then subdividing each of its edges
//Then moves that vertex to the edge of the sphere by radius distance from the center
void Object::subdivideSphere(float radius) {
    int numTriangles;

    vector<float> subbedVertices;
    vector<int> subbedIndices;
    vector<float> subbedTextCord;

    //Loop through the number of subdivisions we want
    int indexs;
    for(int i = 1; i < SPHERE_SUB; ++i){
        indexs = 0;
        numTriangles = indices.size();

        //Subdivide eahc line
        for(int j = 0; j < numTriangles; j += 3){
            int v1 = indices.at(j);
            int v2 = indices.at(j+1);
            int v3 = indices.at(j+2);

            //Calculate each edges midpoint
            glm::vec3 midVec12 = findMidpoint(v1,v2, radius, "vertices");
            glm::vec3 midVec13 = findMidpoint(v1,v3, radius, "vertices");
            glm::vec3 midVec23 = findMidpoint(v2,v3, radius, "vertices");

            //Add new vertices to vector<>
            addVerts(subbedVertices, {vertices.at(v1*3),vertices.at(v1*3 + 1), vertices.at(v1*3 + 2)});

            addVerts(subbedVertices, midVec12);

            addVerts(subbedVertices, {vertices.at(v2*3),vertices.at(v2*3 + 1), vertices.at(v2*3 + 2)});

            addVerts(subbedVertices, midVec13);

            addVerts(subbedVertices, {vertices.at(v3*3),vertices.at(v3*3 + 1), vertices.at(v3*3 + 2)});

            addVerts(subbedVertices, midVec23);


            //Find midpoint for texture coordinates
            glm::vec3 midTex12 = findMidpoint(v1,v2, radius, "textCords");
            glm::vec3 midTex13 = findMidpoint(v1,v3, radius, "textCords");
            glm::vec3 midTex23 = findMidpoint(v2,v3, radius, "textCords");

            //add new textcords
            subbedTextCord.push_back(textCords.at(v1*2));
            subbedTextCord.push_back(textCords.at(v1*2 + 1));
            subbedTextCord.push_back(midTex12.x);
            subbedTextCord.push_back(midTex12.y);
            subbedTextCord.push_back(textCords.at(v2*2));
            subbedTextCord.push_back(textCords.at(v2*2 + 1));
            subbedTextCord.push_back(midTex13.x);
            subbedTextCord.push_back(midTex13.y);
            subbedTextCord.push_back(textCords.at(v3*2));
            subbedTextCord.push_back(textCords.at(v3*2 + 1));
            subbedTextCord.push_back(midTex23.x);
            subbedTextCord.push_back(midTex23.y);

            // Diagram of each triangle's subdivision:
            //         v1
            //        / \
            // newV1 *---* newV3
            //      / \ / \
            //    v2---*---v3
            //       newV2

            //add the 4 triangles to the indices array
            addIndices(subbedIndices, {indexs,indexs+1, indexs+3});
            addIndices(subbedIndices, {indexs+1,indexs+2, indexs+5});
            addIndices(subbedIndices, {indexs+4,indexs+3, indexs+5});
            addIndices(subbedIndices, {indexs+3,indexs+1, indexs+5});

            indexs += 6;

        }

        //set each corresponding vector to their subdivided one
        vertices = subbedVertices;
        indices = subbedIndices;
        textCords = subbedTextCord;
        subbedVertices.clear();
        subbedIndices.clear();
        subbedTextCord.clear();
    }
}

//Finds the midpoint between 2 vertices
glm::vec3 Object::findMidpoint(int vertex1, int vertex2, float radius, string type) {

    glm::vec3 p1Cords = glm::vec3(0.0f);
    glm::vec3 p2Cords = glm::vec3(0.0f);
    glm::vec3 midpoint = glm::vec3(0.0f);

    //checks what type of vertex to be subdivided (vertices = vec3, texCords = vec2)
    if(type == "vertices") {
        p1Cords = {vertices.at(vertex1 * 3), vertices.at(vertex1 * 3 + 1), vertices.at(vertex1 * 3 + 2)};
        p2Cords = {vertices.at(vertex2 * 3), vertices.at(vertex2 * 3 + 1), vertices.at(vertex2 * 3 + 2)};

        midpoint = {(p1Cords.x + p2Cords.x) / 2, (p1Cords.y + p2Cords.y) / 2,(p1Cords.z + p2Cords.z) / 2};

        float scaling = radius / sqrt(midpoint.x * midpoint.x + midpoint.y * midpoint.y + midpoint.z * midpoint.z);
        midpoint *= scaling;
    }else if(type == "textCords"){
        p1Cords = {textCords.at(vertex1 * 2), textCords.at(vertex1 * 2 + 1), 0};
        p2Cords = {textCords.at(vertex2 * 2), textCords.at(vertex2 * 2 + 1), 0};

        midpoint = {(p1Cords.x + p2Cords.x) / 2, (p1Cords.y + p2Cords.y) / 2,0};
    }

    return midpoint;
}


//simplifies adding (float)vec3's to a vector<>
void Object::addVerts(vector<float> &vector, glm::vec3 info) {
    vector.push_back(info.x);
    vector.push_back(info.y);
    vector.push_back(info.z);
}

//simplifies adding (int)vec3's to a vector<>
void Object::addIndices(vector<int> &vector, glm::vec3 info) {
    vector.push_back((int) info.x);
    vector.push_back((int) info.y);
    vector.push_back((int) info.z);

}

//Calculates the objects (vertex) normals
void Object::calculateNormals() {
    for(int i = 0; i < vertices.size(); i += 3){
        //get vertices local position
        glm::vec3 vertPos = {vertices.at(i), vertices.at(i + 1), vertices.at(i + 2)};

        //since the shapes are generated from the middle the vertex normal can simply be calculated
        //by finding its unit vector:
        vertPos.x = vertPos.x / scale.x;
        vertPos.y = vertPos.y / scale.y;
        vertPos.z = vertPos.z / scale.z;

        //add to normal vector<>
        addVerts(normals,vertPos);
    }
}

//Public//////////
/* Object Constructor
 * Holds info about: position, scale, velocity, color, if its dynamic / static, and the screen size
 */
Object::Object(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, float mass, glm::vec3 spin, Shape shape, string textureFilename, glm::vec2 uvScale, bool dynamic, int scWidth, int scHeight){
    this->position = position;
    this->scale = scale;
    this->velocity = velocity;
    this->mass = mass;
    this->spin = spin;
    this->textureFilename = textureFilename;
    this->uvScale = uvScale;
    this->dynamic = dynamic;
    this->scWidth = scWidth;
    this->scHeight = scHeight;
    this->cubemap = false;

    if(shape == Shape::Cube){
        generateCube();
    }else if(shape == Shape::Sphere){
        generateSphere();
    }else{
        cout<< "ERROR: Invalid Shape Entered: " << endl;
        terminate();
    }

    assignBuffandArr();
}

Object::Object(glm::vec3 position, glm::vec3 scale, Shape shape, string textureFilename, glm::vec2 uvScale, bool cubemap, int scWidth, int scHeight){
    this->position = position;
    this->scale = scale;
    this->velocity = glm::vec3(0.0f);
    this->mass = 0.0f;
    this->spin = glm::vec3(0.0f);
    this->textureFilename = textureFilename;
    this->uvScale = uvScale;
    this->cubemap = cubemap;
    this->dynamic = false;
    this->scWidth = scWidth;
    this->scHeight = scHeight;

    if(shape == Shape::Cube){
        generateCube();
    }else if(shape == Shape::Sphere){
        generateSphere();
    }else{
        cout<< "ERROR: Invalid Shape Entered: " << endl;
        terminate();
    }

    assignBuffandArr();
}

//Destructor: deletes the created buffers
Object::~Object() {
    glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &this->eb);
}

/* Basic move
 * if there's a velocity on the object then it updates it from the last frame
 */
void Object::move(float deltaTime) {
    //if there is a force on the object, update its velocity size last frame
    if(currentForce != glm::vec3(0.0f)){
        velocity.x += (currentForce.x / mass) * deltaTime;
        velocity.y += (currentForce.y / mass) * deltaTime;
        velocity.z += (currentForce.z / mass) * deltaTime;

    }

    //if the object is spinning update its local rotation since last frame
    if(spin != glm::vec3(0.0f)){
        angle.x += spin.x * deltaTime;
        angle.y += spin.y * deltaTime;
        angle.z += spin.z * deltaTime;
    }

    //if the object is dynamic then update the position since last frame given its velocity
    if(dynamic) {
        if (velocity.x != 0) {
            position.x += velocity.x * deltaTime;
        }
        if (velocity.y != 0) {
            position.y += velocity.y * deltaTime;
        }
        if(velocity.z != 0){
            position.z += velocity.z * deltaTime;
        }
    }
}

/*Display
 * Applys any transformations the object has had and displays the object
 */
void Object::display(Shader* shader) {

    //If this object is a cubemap then display the inside faces:
    if(!cubemap){
        glCullFace(GL_FRONT);
    }else{
        glCullFace(GL_BACK);
    }


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //Calculating Transforms:
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));

    //Setting Uniforms:
    shader->setInt("texture1", 0);
    shader->setUniformMat4("model", model);

    //Displaying object
    glBindVertexArray(this->va); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT, 0);

}

void Object::addForce(glm::vec3 otherObjPos, float otherObjMass) {
    float dist = 1000000 * sqrt(otherObjPos.x*otherObjPos.x  + otherObjPos.y*otherObjPos.y + otherObjPos.z*otherObjPos.z);
//    float dist = 100 * sqrt(otherObjPos.x*otherObjPos.x  + otherObjPos.y*otherObjPos.y + otherObjPos.z*otherObjPos.z);

    float totalForce = UNIVERSAL_GRAVITY_CONSTANT * mass * otherObjMass / (dist*dist);


    currentForce += glm::vec3(totalForce * (otherObjPos.x - position.x), totalForce * (otherObjPos.y - position.y), totalForce * (otherObjPos.z - position.z));
}


/*getPostion
 * returns the objects position as a vec3
 */
glm::vec3 Object::getPosition() {
    return position;
}

/*getVelocity
 * returns the objects velocity as a vec3
 */
glm::vec3 Object::getVelocity() {
    return velocity;
}

/*getScale
 * returns the objects scale as a vec3
 */
glm::vec3 Object::getScale() {
    return scale;
}

float Object::getMass() {
    return mass;
}

/* setVelocity
 * sets the velocity to the inputted vec2
 */
void Object::setVelocity(glm::vec3 newVelocity) {
    this->velocity = newVelocity;
}

void Object::setAngle(glm::vec3 add) {
    this->angle.x += add.x;
    this->angle.y += add.y;
    this->angle.z += add.z;

}

void Object::setForce(glm::vec3 force){
    currentForce = force;
}
