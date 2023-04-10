//
// Created by brado on 3/21/2023.
//

#include "Shader.h"
#include <fstream>
#include <sstream>

/*Constructor
 * recieves vertex / fragment shader files, then calls read to read them into strings
 * then creates the shader using those strings
 */
Shader::Shader(const string &vertexSource, const string &fragSource) {
    string vert = readFile(vertexSource);
    string frag = readFile(fragSource);

    createShader(vert.c_str(),frag.c_str());
}

//Destructor: deletes the created shader
Shader::~Shader() {
    glDeleteProgram(program);
}

/*readFile
 * reads in the shader files and returns them as string
 */
string Shader::readFile(const string &fileName) {
    ifstream stream(fileName);

    string line;
    stringstream ss;

    while(getline(stream, line)){
        ss << line << '\n';
    }
    return ss.str();
}

/*createShader
 * generates the shader based off the parsed shader files as strings
 */
void Shader::createShader(const char *vertSource, const char *fragSource) {
    //Applies the .glsl veraion to be used
    unsigned int vert, frag;
    const char* version = "#version 330";
    const char* vSources[] = {
            version,
            vertSource
    };
    const char* fSources[] = {
            version,
            fragSource
    };

    //create vertex shader and check for errors
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 2, vSources, NULL);
    glCompileShader(vert);
    findErrors(vert, 'v');

    //create fragment shader and check for errors
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 2, fSources, NULL);
    glCompileShader(frag);
    findErrors(frag, 'f');

    //Combine and link shaders to the program
    this->program = glCreateProgram();
    glAttachShader(this->program, vert);
    glAttachShader(this->program, frag);
    glLinkProgram(this->program);
    findErrors(program, 'p');

    //since they have been binded to program we can delete the shaders
    glDeleteShader(vert);
    glDeleteShader(frag);
}

/*findErrors
 * checks to see if there has been any errors in the creation of the shaders
 */
void Shader::findErrors(unsigned int errorCheck, char type) {
    int success;
    char infoLog[512];

    //determines the type of error to be checked
    if(type == 'p'){
        glGetProgramiv(errorCheck, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(errorCheck, 512, NULL, infoLog);
            cout << "ERROR : Shader : Program-Compile error\n" << infoLog << endl;
        }
    }else{
        glGetShaderiv(errorCheck, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(errorCheck, 512, NULL, infoLog);
            cout << "ERROR : Shader : Shader-Link error of Type: " << (type == 'v' ? "Vertex" : "Fragment") << "\n" << infoLog << endl;
        }
    }
}

/*bindShader
 * binds the shader, so that whatever is drawn after with use this shader
 */
void Shader::bindShader() {
    glUseProgram(this->program);
}

/*setUniformMat4
 * allows the program to access the vertex shader and pass a matrix 4 uniform
 * based off the variable name entered
 */
void Shader::setUniformMat4(const char *variableName, glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(this->program, variableName), 1, false, glm::value_ptr(matrix));
}

/*setVec3
 * allows the program to access the vertex shader and pass a vec3 uniform
 * based off the variable name entered
 */
void Shader::setVec3(const char *variableName, glm::vec3 vector) {
    glUniform3f(glGetUniformLocation(this->program, variableName), vector.x, vector.y, vector.z);
}

void Shader::setInt(const char *variableName, int integer){
    glUniform1i(glGetUniformLocation(this->program, variableName), integer);
}



