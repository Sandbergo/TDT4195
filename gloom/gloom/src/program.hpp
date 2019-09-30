#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#pragma once


// System headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <sceneGraph.hpp>
#include <glm/vec3.hpp>
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"


// Main OpenGL program
void runProgram(GLFWwindow* window);


// Function for handling keypresses
void handleKeyboardInput(GLFWwindow* window);


// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
inline void printGLError() {
    int errorID = glGetError();

    if(errorID != GL_NO_ERROR) {
        std::string errorString;

        switch(errorID) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            default:
                errorString = "[Unknown error ID]";
                break;
        }

        fprintf(stderr, "An OpenGL error occurred (%i): %s.\n",
                errorID, errorString.c_str());
    }
}


// create VAO object 

int setUpVAOtriangle(std::vector<float> vertexVec, 
                     std::vector<float> colorVec,
                     std::vector<float> normalVec,
                     std::vector<unsigned int> indexVec);


void updateMVP();

SceneNode* createSceneGraph();

void drawSceneNode(SceneNode* root, glm::mat4 viewProjectionMatrix, Gloom::Shader* shader);

void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar);

#endif
