//spinesquare

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "Object.h"
#include "Camera.h"

using namespace std;


const int SC_WIDTH = 1600;
const int SC_HEIGHT = 1600;

void checkInput(GLFWwindow *window, Object &object, float deltaTime){

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        //camera.move({0.0, 0.0, -1.0}, deltaTime);
        object.setVelocity(glm::vec3(0.0,0.0,-0.75));
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        //camera.move({0.0, 0.0, 1.0}, deltaTime);
        object.setVelocity(glm::vec3(0.0,0.0,0.75));
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //camera.move({-1.0, 0.0, 0.0}, deltaTime);
        object.setVelocity(glm::vec3(-0.75,0.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        //camera.move({1.0, 0.0, 0.0}, deltaTime);
        object.setVelocity(glm::vec3(0.75,0.0,0.0));
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        object.setVelocity(glm::vec3(0.0,-0.75,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        object.setVelocity(glm::vec3(0.0,0.75,0.0));
    }else{
        object.setVelocity(glm::vec3(0.0,0.0,0.0));
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        object.setAngle(0.1);
    }else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        object.setAngle(-0.1);
    }
}


int main(){
    //Window initialization:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window;
    window = glfwCreateWindow(SC_WIDTH, SC_HEIGHT, "Basic Game", NULL, NULL);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //make sure the window was created
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize glew to utilize OpenGl
    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize glew" << endl;
        glfwTerminate();
        return -1;
    }


    Shader* shader = new Shader("../vertexShader.glsl", "../fragmentShader.glsl");
    Camera camera = *new Camera({0.0,0.0,3.0}, {0.0,0.0,-1.0},{0.0,1.0,0.0});

    Object plane = *new Object(glm::vec3(1.0, 1.0,-3.0), glm::vec3(0.5,0.5,0.5), glm::vec3(0.0,0.0,0.0), 10.0f, glm::vec3(1.0f, 0.5f, 0.2f), Shape::Cube, true, SC_WIDTH, SC_HEIGHT);
    //Object plane2 = *new Object(glm::vec3(-0.5, 0.0,-3.0), glm::vec3(0.5,0.5,0.5), glm::vec3(0.0,0.0,0.0), glm::vec3(1.0f, 0.5f, 0.2f), false, SC_WIDTH, SC_HEIGHT);

    glm::mat4 projection = glm::mat4(1.0f);
    auto startTime = std::chrono::high_resolution_clock::now();
    //Game Loop:
    while(!glfwWindowShouldClose(window)){
        auto endTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        deltaTime *= 0.001;

        startTime = std::chrono::high_resolution_clock::now();

        checkInput(window, plane, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bindShader();

        projection = glm::perspective(glm::radians(45.0f), (float) SC_WIDTH / (float) SC_HEIGHT, 0.1f, 100.0f);


        shader->setUniformMat4("projection", projection);
        camera.applyView(shader);

        plane.move(deltaTime);

        plane.display(shader);
        //plane2.display(shader);

        //refresh the window
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
}
