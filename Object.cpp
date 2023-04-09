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
    //glCullFace(GL_BACK);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // texture 1
    // ---------
    string filename = "../textures/Ground047_4K_Color.jpg";

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
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
    vector<float> verts;

    vector<int> seam = {0,1,6,12};

    //Initialize verts, since we know what the triangles will be prior to subdivision
    indices = {
            0,1,2,
            0,2,3,
            0,3,4,
            0,4,5,
            0,5,1,
            1,5,6,
            2,1,7,
            3,2,8,
            4,3,9,
            5,4,10,
            6,7,1,
            7,8,2,
            8,9,3,
            9,10,4,
            10,6,5,
            11,7,6,
            11,8,7,
            11,9,8,
            11,10,9,
            11,6,10
    };

    verts.push_back(0);
    verts.push_back(scale.y/2);
    verts.push_back(0);
    float phi;
    float theta;
    int i;
    for(i = 1; i < 11; ++i){
        if(i < 6){
            phi = atan(0.5f);
            theta = 2*M_PI/5 * i;
        }else{
            phi = -atan(0.5f);
            theta = 2*M_PI/5 * i - M_PI/5;
        }
        float x = radius * cos(phi) * cos(theta);
        float y = radius * sin(phi);
        float z = radius * cos(phi) * sin(theta);

        verts.push_back(x);
        verts.push_back(y);
        verts.push_back(z);
    }
    verts.push_back(0);
    verts.push_back(-scale.y/2);
    verts.push_back(0);

    vertices = verts;
    cout << "pre sub vertices: {" << endl;
    for(i = 0; i < vertices.size(); ++i){
        cout << vertices.at(i) << ", ";
        if((i+1) % 3 == 0){
            cout << endl;
        }
    }
    cout << "}\npre sub indices: {"<<endl;
    for(i = 0; i < indices.size(); ++i){
        cout << indices.at(i) << ", ";
        if((i+1) % 3 == 0){
            cout << endl;
        }
    }
    cout << "indices length: " << indices.size() << endl;
    subdivideSphere(radius, seam);
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
            1, 3, 0,  // first Triangle
            3, 1, 2,   // second Triangle
            6, 4, 7,
            4, 6, 5,
            9, 4, 5,
            4, 9, 8,
            6, 13, 12,
            13, 6, 7,
            3, 4, 0,
            4, 3, 7,
            10, 6, 11,
            6, 10, 5
    };

    textCords = {
            1.0,1.0,
            0.0,1.0,
            0.0,0.0,
            1.0,0.0,

            2.0,1.0,
            3.0,1.0,
            3.0,0.0,
            2.0,0.0,

            2.0,2.0,

            3.0,2.0,
            4.0,1.0,

            4.0,0.0,
            3.0,-1.0,

            2.0,-1.0
    };
}



void Object::subdivideSphere(float radius, vector<int> &seam) {
    int numTriangles = indices.size();

    vector<float> subbedVertices;
    vector<int> subbedIndices;
    vector<int> subbedSeam;

    int indexs;

    cout << "v1, v2, v3: "<<endl;

    for(int i = 1; i < 2; ++i){
        indexs = 0;
        for(int j = 0; j < numTriangles; j += 3){
            int v1 = indices.at(j);
            int v2 = indices.at(j+1);
            int v3 = indices.at(j+2);


            glm::vec3 midVec12 = findMidpoint(v1,v2, radius);
            glm::vec3 midVec13 = findMidpoint(v1,v3, radius);
            glm::vec3 midVec23 = findMidpoint(v2,v3, radius);

            if(v1 == seam.at(i) && v2 == seam.at(i)){
                
            }else if(v1 == seam.at(i) && v3 == seam.at(i)){

            }else if(v2 == seam.at(i) && v3 == seam.at(i)){

            }

            addVerts(subbedVertices, {vertices.at(v1*3),vertices.at(v1*3 + 1), vertices.at(v1*3 + 2)});

            addVerts(subbedVertices, midVec12);

            addVerts(subbedVertices, {vertices.at(v2*3),vertices.at(v2*3 + 1), vertices.at(v2*3 + 2)});

            addVerts(subbedVertices, midVec13);

            addVerts(subbedVertices, {vertices.at(v3*3),vertices.at(v3*3 + 1), vertices.at(v3*3 + 2)});

            addVerts(subbedVertices, midVec23);

            // compute 3 new vertices by spliting half on each edge
            //         v1
            //        / \
            // newV1 *---* newV3
            //      / \ / \
            //    v2---*---v3
            //       newV2


            addIndices(subbedIndices, {indexs+1,indexs, indexs+3});
            addIndices(subbedIndices, {indexs+2,indexs+1, indexs+5});
            addIndices(subbedIndices, {indexs+3,indexs+4, indexs+5});
            addIndices(subbedIndices, {indexs+1,indexs+3, indexs+5});

            indexs += 6;

        }
        vertices = subbedVertices;
        indices = subbedIndices;
        cout << "indices length: " << indices.size() << endl;
    }
}

glm::vec3 Object::findMidpoint(int vertex1, int vertex2, float radius) {
    glm::vec3 vertex1Cords = {vertices.at(vertex1*3), vertices.at(vertex1*3 + 1), vertices.at(vertex1*3 + 2)};
    glm::vec3 vertex2Cords = {vertices.at(vertex2*3), vertices.at(vertex2*3 + 1), vertices.at(vertex2*3 + 2)};

    glm::vec3 midpoint = {(vertex1Cords.x + vertex2Cords.x)/2, (vertex1Cords.y + vertex2Cords.y)/2, (vertex1Cords.z + vertex2Cords.z)/2};

    float scaling = radius / sqrt(midpoint.x*midpoint.x + midpoint.y*midpoint.y + midpoint.z*midpoint.z);
    midpoint *= scaling;

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

//Public//////////
/* Object Constructor
 * Holds info about: position, scale, velocity, color, if its dynamic / static, and the screen size
 */
Object::Object(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, float mass, glm::vec3 color, Shape shape, bool dynamic, int scWidth, int scHeight){
    this->position = position;
    this->scale = scale;
    this->velocity = velocity;
    this->mass = mass;
    this->color = color;
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
