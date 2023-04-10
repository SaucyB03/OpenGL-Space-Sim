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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + textCords.size() * sizeof(float), 0, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices.front());
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), textCords.size() * sizeof(float), &textCords.front());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_DYNAMIC_DRAW);
    }else {
        glBindBuffer(GL_ARRAY_BUFFER, this->vb);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + textCords.size() * sizeof(float), 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices);
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), textCords.size() * sizeof(float), &textCords);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);
    }

    //Assign vertex attribute pointer telling OpenGL how the array of verts is organized
    //i.e tell it if there is an offset or a stride it needs to be aware of
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertices.size() * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Unbinds the buffer and vertex array now that we are done with them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // texture 1
    // ---------
    string filename = "../textures/UVMap.jpg";

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters`
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Object::generateSphere() {
    float radius = scale.x/2;

    float phi, theta, LRot, x, y, z, tx = 0.0, ty = 0.0;
    int i, index = 0;
    for(i = 1; i <= 12; ++i) {
        phi = atan(0.5f);
        theta = 2 * M_PI / 5 * i - LRot;

        if (i < 6) {
            x = radius * cos(phi) * cos(theta);
            y = radius * sin(phi);
            z = radius * cos(phi) * sin(theta);

            addIndices(indices, {index + 1, index, index + 2});
            addIndices(indices, {index + 2, index, index + 11});
            addVerts(vertices, {x, y, z});
            addVerts(vertices, {0.0, scale.y / 2, 0.0});

            textCords.push_back(tx * uvScale);
            textCords.push_back(ty * uvScale);
            textCords.push_back((tx + 0.5f) * uvScale);
            textCords.push_back((ty - 1.0f) * uvScale);

            tx ++;

            index += 2;
        } else if (i > 6 && i < 12) {
            x = radius * cos(-phi) * cos(theta);
            y = radius * sin(-phi);
            z = radius * cos(-phi) * sin(theta);

            addIndices(indices, {index - 1, index, index + 1});
            addIndices(indices, {index - 10, index - 1, index + 1});
            addVerts(vertices, {x, y, z});
            addVerts(vertices, {0.0, -scale.y / 2, 0.0});

            textCords.push_back(tx * uvScale);
            textCords.push_back(ty * uvScale);
            textCords.push_back((tx + 0.5f) * uvScale);
            textCords.push_back((ty + 1.0f) * uvScale);

            tx ++;

            index += 2;
        } else if(i == 12){
            x = radius * cos(-phi) * cos(theta);
            y = radius * sin(-phi);
            z = radius * cos(-phi) * sin(theta);

            addVerts(vertices, {x, y, z});
            textCords.push_back(tx * uvScale);
            textCords.push_back(ty * uvScale);
        }else{
            x = radius * cos(phi) * cos(theta);
            y = radius * sin(phi);
            z = radius * cos(phi) * sin(theta);

            addVerts(vertices, {x, y, z});
            textCords.push_back(tx * uvScale);
            textCords.push_back(ty * uvScale);

            tx = 1.0;
            ty = 1.0;

            LRot = M_PI/5;
            index += 2;
        }
    }

    subdivideSphere(radius);

    //calculateTexCordsAndNormals(seam);
}

void Object::generateCube() {
    //  6*------------*5
    //  /|           /|
    //2*------------*1|
    // | |          | |
    // |7*----------|-*4
    // |/           |/
    //3*------------*0


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
            scale.x/2,  -scale.y/2,    scale.z/2,

            scale.x/2,  scale.y/2,    scale.z/2,
            scale.x/2,  scale.y/2,    scale.z/2,

            -scale.x/2,  scale.y/2,    scale.z/2,
            -scale.x/2,  scale.y/2,    scale.z/2,

            -scale.x/2,  -scale.y/2,   scale.z/2

    };
    indices = {
            3, 1, 0,  // first Triangle
            1, 3, 2,   // second Triangle
            4, 6, 7,
            6, 4, 5,
            4, 9, 5,
            9, 4, 8,
            13, 6, 12,
            6, 13, 7,
            4, 3, 0,
            3, 4, 7,
            6, 10, 11,
            10, 6, 5
    };

    textCords = {
            uvScale,uvScale,
            0.0,uvScale,
            0.0,0.0,
            uvScale,0.0,

            2*uvScale,uvScale,
            3*uvScale,uvScale,
            3*uvScale,0.0,
            2*uvScale,0.0,

            2*uvScale,2*uvScale,

            3*uvScale,2*uvScale,
            4*uvScale,uvScale,

            4*uvScale,0.0,
            3*uvScale,-uvScale,

            2*uvScale,-uvScale
    };
}



void Object::subdivideSphere(float radius) {
    int numTriangles;

    vector<float> subbedVertices;
    vector<int> subbedIndices;
    vector<float> subbedTextCord;

    int indexs;
    for(int i = 1; i < 4; ++i){
        indexs = 0;
        numTriangles = indices.size();
        for(int j = 0; j < numTriangles; j += 3){
            int v1 = indices.at(j);
            int v2 = indices.at(j+1);
            int v3 = indices.at(j+2);


            glm::vec3 midVec12 = findMidpoint(v1,v2, radius, "vertices");
            glm::vec3 midVec13 = findMidpoint(v1,v3, radius, "vertices");
            glm::vec3 midVec23 = findMidpoint(v2,v3, radius, "vertices");

            addVerts(subbedVertices, {vertices.at(v1*3),vertices.at(v1*3 + 1), vertices.at(v1*3 + 2)});

            addVerts(subbedVertices, midVec12);

            addVerts(subbedVertices, {vertices.at(v2*3),vertices.at(v2*3 + 1), vertices.at(v2*3 + 2)});

            addVerts(subbedVertices, midVec13);

            addVerts(subbedVertices, {vertices.at(v3*3),vertices.at(v3*3 + 1), vertices.at(v3*3 + 2)});

            addVerts(subbedVertices, midVec23);

            glm::vec3 midTex12 = findMidpoint(v1,v2, radius, "textCords");
            glm::vec3 midTex13 = findMidpoint(v1,v3, radius, "textCords");
            glm::vec3 midTex23 = findMidpoint(v2,v3, radius, "textCords");

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

            // compute 3 new vertices by spliting half on each edge
            //         v1
            //        / \
            // newV1 *---* newV3
            //      / \ / \
            //    v2---*---v3
            //       newV2


            addIndices(subbedIndices, {indexs,indexs+1, indexs+3});
            addIndices(subbedIndices, {indexs+1,indexs+2, indexs+5});
            addIndices(subbedIndices, {indexs+4,indexs+3, indexs+5});
            addIndices(subbedIndices, {indexs+3,indexs+1, indexs+5});

            indexs += 6;

        }
        vertices = subbedVertices;
        indices = subbedIndices;
        textCords = subbedTextCord;
        subbedVertices.clear();
        subbedIndices.clear();
        subbedTextCord.clear();
    }
}

glm::vec3 Object::findMidpoint(int vertex1, int vertex2, float radius, string type) {

    glm::vec3 p1Cords = glm::vec3(0.0f);
    glm::vec3 p2Cords = glm::vec3(0.0f);
    glm::vec3 midpoint = glm::vec3(0.0f);

    if(type == "vertices") {
        p1Cords = {vertices.at(vertex1 * 3), vertices.at(vertex1 * 3 + 1), vertices.at(vertex1 * 3 + 2)};
        p2Cords = {vertices.at(vertex2 * 3), vertices.at(vertex2 * 3 + 1), vertices.at(vertex2 * 3 + 2)};

        midpoint = {(p1Cords.x + p2Cords.x) / 2, (p1Cords.y + p2Cords.y) / 2,
                              (p1Cords.z + p2Cords.z) / 2};

        float scaling = radius / sqrt(midpoint.x * midpoint.x + midpoint.y * midpoint.y + midpoint.z * midpoint.z);
        midpoint *= scaling;
    }else if(type == "textCords"){
        p1Cords = {textCords.at(vertex1 * 2), textCords.at(vertex1 * 2 + 1), 0};
        p2Cords = {textCords.at(vertex2 * 2), textCords.at(vertex2 * 2 + 1), 0};

        midpoint = {(p1Cords.x + p2Cords.x) / 2, (p1Cords.y + p2Cords.y) / 2,0};
    }

    return midpoint;
}



void Object::addVerts(vector<float> &vector, glm::vec3 info) {
    vector.push_back(info.x);
    vector.push_back(info.y);
    vector.push_back(info.z);
}

void Object::addIndices(vector<int> &vector, glm::vec3 info) {
    vector.push_back((int) info.x);
    vector.push_back((int) info.y);
    vector.push_back((int) info.z);

}

void Object::calculateNormals() {
    int i;
    for(i = 0; i < indices.size(); ++i){

    }
}

//Public//////////
/* Object Constructor
 * Holds info about: position, scale, velocity, color, if its dynamic / static, and the screen size
 */
Object::Object(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, float mass, glm::vec3 color, Shape shape, float uvScale, bool dynamic, int scWidth, int scHeight){
    this->position = position;
    this->scale = scale;
    this->velocity = velocity;
    this->mass = mass;
    this->color = color;
    this->uvScale = uvScale;
    this->dynamic = dynamic;
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
    //Calculating Transforms:
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first


    model = glm::translate(model, position);
    //model = glm::translate(model, {-scale.x/2, 0.0,-scale.z/2});
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::translate(model, {scale.x/2, 0.0,scale.z/2});

    shader->setInt("texture1", 0);
    shader->setUniformMat4("model", model);
    shader->setVec3("color", color);

    //Displaying object
    glBindVertexArray(this->va); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT, 0);

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

/* setVelocity
 * sets the velocity to the inputted vec2
 */
void Object::setVelocity(glm::vec3 newVelocity) {
    this->velocity = newVelocity;
}

void Object::setAngle(float add) {
    this->angle += add;
}
