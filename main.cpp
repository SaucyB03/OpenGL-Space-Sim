
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "Object.h"
#include "Camera.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace std;


//Initialize basic variables
int SC_WIDTH = 3200;
int SC_HEIGHT = 1600;

Camera* camera;

//Keyboard callback function: checks the user keyinput
void checkInput(GLFWwindow *window, float deltaTime){

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }


    //camera movement:
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

//Mouse callback function: checks for users mouse movement and then passes that information to the camera
void mouseCallback(GLFWwindow *window, double x, double y){
    camera->mouseCallback(window,x,y);
}

int main(){
    //Initialize Window
    //Window initialization:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

    //Specifying the monitor to be displayed on + setting the sc size to be fullscreen
    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    const GLFWvidmode *mode = glfwGetVideoMode(monitors[0]);
    SC_WIDTH = mode->width;
    SC_HEIGHT = mode->height;
    GLFWwindow* window;
    window = glfwCreateWindow(SC_WIDTH, SC_HEIGHT, "Space Simulation", monitors[0], NULL);

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

// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
// Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("1.0.72");
// Setup Dear ImGui style
    ImGui::StyleColorsDark();


    //Initialize Shaders:
    Shader* lightSourceshader = new Shader("../vertexLightSourceShader.glsl", "../fragmentLightSourceShader.glsl");
    Shader* shader = new Shader("../vertexShader.glsl", "../fragmentShader.glsl");

    //Define the global camera object
    camera = new Camera({-100.0,100.0,0.0}, {0.0,-1.0,0.0},{0.0,1.0,0.0});

    //Texture Files:
    string sunTex = "../textures/ExaggeratedSunTexture.jpg";
    string planetTex = "../textures/Moss_Dirt_Planet.jpg";
    string uvTex = "../textures/UVMap.jpg";
    string cubeMap = "../textures/cubemap.png";
    string earthTex = "../textures/EarthIcoTexture.png"; // must set objects uvScale to (0.181818,0.333333) for texture to appear correct...   5.5 triangles wide, 3 triangles tall : (1/5.5, 1/3)


    //Planets apart of the scene (if not stored in the vector, force on other objects wont be applied)
    //Index 0 is the sun
    vector<Object*> planets;
    planets.push_back(new Object(glm::vec3(0.0, 3,0.0), glm::vec3(13.92,13.92,13.92), glm::vec3(0.0,0.0,0.0), 1988500.0f *pow(10,16), glm::vec3(0.0f, 0.0f, 0.0f), Shape::Sphere, sunTex, glm::vec2(1.0f,1.0f), false, SC_WIDTH, SC_HEIGHT));
//    planets.push_back(new Object(glm::vec3(152.10, 3,0.0), glm::vec3(1.2756,1.2756,1.2756), glm::vec3(0.0,0.0,40.0), 5.9724 * pow(10,16), glm::vec3(0.0f, 40.0f, 0.0f), Shape::Sphere, earthTex, glm::vec2(0.181818,0.333333), true, SC_WIDTH, SC_HEIGHT));
//    planets.push_back(new Object(glm::vec3(152.10, 3,3.0), glm::vec3(0.75,0.75,0.75), glm::vec3(1.0,0.0,40.0), 2.9724 * pow(10,16), glm::vec3(0.0f, 0.0f, 0.0f), Shape::Sphere, planetTex, glm::vec2(1.0,1.0), true, SC_WIDTH, SC_HEIGHT));
//    planets.push_back(new Object(glm::vec3(-100.0, 3,3.0), glm::vec3(3,3,3), glm::vec3(0.0,0.0,30.0), 2.9724 * pow(10,16), glm::vec3(0.0f, 0.0f, 0.0f), Shape::Sphere, planetTex, glm::vec2(1.0,1.0), true, SC_WIDTH, SC_HEIGHT));

//    planets.push_back(new Object(glm::vec3(0.0, 3,0.0), glm::vec3(13.92,13.92,13.92), glm::vec3(0.0,0.0,0.0), 1988500.0f *pow(10,16), glm::vec3(0.0f, 0.0f, 0.0f), Shape::Sphere, sunTex, glm::vec2(1.0f,1.0f), false, SC_WIDTH, SC_HEIGHT));
//    planets.push_back(new Object(glm::vec3(100, 3,0.0), glm::vec3(13.92,13.92,13.92), glm::vec3(0.0,0.0,40.0), 1988500.0f * pow(10,16), glm::vec3(0.0f, 40.0f, 0.0f), Shape::Sphere, earthTex, glm::vec2(0.181818,0.333333), true, SC_WIDTH, SC_HEIGHT));



    //Skybox background img object
    Object skybox = *new Object(glm::vec3(0,0.0,0.0), glm::vec3(500,500,500), Shape::Cube, cubeMap, glm::vec2(0.25f,0.3333333f), true, SC_WIDTH, SC_HEIGHT);
    glfwSetCursorPosCallback(window, mouseCallback);


    glm::mat4 projection = glm::mat4(1.0f);
    auto startTime = std::chrono::high_resolution_clock::now();

    //Game Loop:
    while(!glfwWindowShouldClose(window)){
        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        //Calculate deltaTime:
        auto endTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        deltaTime *= 0.001;

        startTime = std::chrono::high_resolution_clock::now();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        checkInput(window, deltaTime);

        //Calculate the current cameras perspective:
        projection = glm::perspective(glm::radians(45.0f), (float) SC_WIDTH / (float) SC_HEIGHT, 1.0f, 1000.0f);


        //Loop through planets and applys the forces on each other
        int i,j;
        for(i = 0; i < planets.size(); ++i){
            planets.at(i)->setForce({0.0,0.0,0.0});
            for(j = 0; j < planets.size(); ++j){
                if(j != i){
                    planets.at(i)->addForce(planets.at(j)->getPosition(), planets.at(j)->getMass());
                }
            }
        }

        //Update the Planets movement
        for(i = 0; i < planets.size(); ++i){
            planets.at(i)->move(deltaTime);
        }

        //Bind the Sun's Shader + set its Uniforms and display
        lightSourceshader->bindShader();

        lightSourceshader->setUniformMat4("projection", projection);
        camera->applyView(lightSourceshader);
        planets.at(0)->display(lightSourceshader);

        //Bind the planets shader + set its Uniforms
        shader->bindShader();
        shader->setUniformMat4("projection", projection);
        shader->setVec3("lightPos", planets.at(0)->getPosition());

        //Apply the camera view (camera position + view direction)
        camera->applyView(shader);

        //Display the Planets
        for(i = 1; i < planets.size(); ++i){
            planets.at(i)->display(shader);
        }


        //Display the Background
        skybox.display(shader);

        // render your GUI
        ImGui::Begin("Demo window");
        ImGui::Text("Hello!");
        ImGui::End();

        // Render dear imgui into screen
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //refresh the window
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
