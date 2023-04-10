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

Camera* camera;

void checkInput(GLFWwindow *window, float deltaTime){

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera->move({0.0, 0.0, 1.0}, deltaTime);
        //object.setVelocity(glm::vec3(0.0,0.0,-0.75));
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera->move({0.0, 0.0, -1.0}, deltaTime);
        //object.setVelocity(glm::vec3(0.0,0.0,0.75));
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->move({-1.0, 0.0, 0.0}, deltaTime);
        //object.setVelocity(glm::vec3(-0.75,0.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera->move({1.0, 0.0, 0.0}, deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera->move({0.0,-1.0,0.0}, deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera->move({0.0,1.0,0.0}, deltaTime);
    }

}

void mouseCallback(GLFWwindow *window, double x, double y){
    camera->mouseCallback(window,x,y);
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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glEnable(GL_DEPTH_TEST);




    Shader* shader = new Shader("../vertexLightSourceShader.glsl", "../fragmentLightSourceShader.glsl");
    camera = new Camera({0.0,0.0,3.0}, {0.0,0.0,-1.0},{0.0,1.0,0.0});

    vector<Object*> planets;
    planets.push_back(new Object(glm::vec3(0.5, 0.5,-1.0), glm::vec3(0.5,0.5,0.5), glm::vec3(0.0,0.0,0.0), 10.0f, glm::vec3(1.0f, 0.5f, 0.2f), Shape::Sphere, 1.0f, true, SC_WIDTH, SC_HEIGHT));
    planets.push_back(new Object(glm::vec3(-0.5, 0.5,-3.0), glm::vec3(0.5,0.5,0.5), glm::vec3(0.0,0.0,0.0), 10.0f, glm::vec3(1.0f, 0.5f, 0.2f), Shape::Cube, 1.0f, true, SC_WIDTH, SC_HEIGHT));

    glfwSetCursorPosCallback(window, mouseCallback);

    glm::mat4 projection = glm::mat4(1.0f);
    auto startTime = std::chrono::high_resolution_clock::now();
    //Game Loop:
    while(!glfwWindowShouldClose(window)){
        auto endTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        deltaTime *= 0.001;

        startTime = std::chrono::high_resolution_clock::now();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        checkInput(window, deltaTime);

        shader->bindShader();

        projection = glm::perspective(glm::radians(45.0f), (float) SC_WIDTH / (float) SC_HEIGHT, 0.1f, 100.0f);


        shader->setUniformMat4("projection", projection);
        shader->setVec3("lightPos", planets.at(0)->getPosition());
        camera->applyView(shader);

        //plane.move(deltaTime);

//        int i,j;
//        for(i = 0; i < planets.size(); ++i){
//            for(j = 0; j < planets.size(); ++j){
//                if(j != i){
//                    planets.at(i)
//                }
//            }
//        }

        planets.at(0)->display(shader);
        planets.at(1)->display(shader);

        //refresh the window
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
}
