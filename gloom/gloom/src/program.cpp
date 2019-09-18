// Local headers
#include "program.hpp"
#include "gloom/shader.hpp"
#include "gloom/gloom.hpp"
#include "math.h"
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

// current motion and rotation vectors
glm::vec3 motionVec = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 rotationVec = glm::vec3(0.0f,0.0f,0.0f);

void runProgram(GLFWwindow* window)
{
    // setup Shader
    Gloom::Shader shader;
    shader.attach("../gloom/shaders/simple.vert");
    shader.attach("../gloom/shaders/simple.frag");
    shader.link();

    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)
    std::vector<float> coordinateVec2 = {
        -0.8, -0.8, 0.0,    -0.4, -0.8, 0.0,    0.8, -0.4, 0.0, 
         0.8, 0.8, 0.0,      0.4, 0.8, 0.0,     0.8, 0.4, 0.0, 
         0.0, 0.0, 0.0,      1.0, 0.0, 0.0,     0.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,     -1.0, 0.0, 0.0,     0.0, 1.0, 0.0,
         0.0, 0.0, 0.0,     -1.0, 0.0, 0.0,     0.0, -1.0, 0.0,
    };
    
    std::vector<unsigned int> indexVec2 = 
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    std::vector<float> colorVec2 = {
        1.0, 0.9, 1.0, 0.6,     0.1, 0.1, 0.1, 0.6,      1.0, 1.0, 0.0, 0.6,
        0.7, 0.6, 1.0, 0.4,     1.0, 1.0, 1.0, 0.4,      1.0, 0.0, 0.6, 0.4,
        1.0, 0.4, 1.0, 0.5,     0.6, 0.4, 1.0, 0.9,      0.7, 0.4, 1.0, 0.5,
        0.7, 0.4, 0.0, 0.9,     1.0, 0.9, 0.3, 0.8,      1.0, 0.4, 0.3, 0.9,
        0.6, 0.4, 1.0, 0.8,     0.1, 0.1, 0.1, 0.6,      0.0, 0.4, 0.6, 0.8,
    };

    std::vector<float> coordinateVec = {
         0.0, 0.0, -0.0,      1.0, 0.0, -0.0,     0.0, 1.0, -0.0,
        -0.8, -0.8, -0.5,     0.8, -0.8, -0.5,    0.0, 0.8, -0.5, 
         0.8, 0.8, 0.5,      -0.8, 0.8, 0.5,      0.8, -0.8, 0.5, 
    };
    
    std::vector<unsigned int> indexVecOld = 
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, };

    std::vector<unsigned int> indexVec = 
    {6,7,8,3,4,5,0,1,2};

    std::vector<float> colorVec = {
        0.0, 1.0, 0.0, 0.3,     0.0, 1.0, 0.0, 0.3,      0.0, 1.0, 0.0, 0.3,
        0.0, 0.0, 1.0, 0.3,     0.0, 0.0, 1.0, 0.3,      0.0, 0.0, 1.0, 0.3,
        1.0, 0.0, 0.0, 0.3,     1.0, 0.0, 0.0, 0.3,      1.0, 0.0, 0.0, 0.3,
    };

    int index = setUpVAOtriangle(coordinateVec, indexVec, colorVec);

    // transformation matrices
    glm::mat4x4 MVPmatrix = glm::mat4(1.0f); 
    glm::mat4x4 perspMatrix = glm::perspective(2.0, 4.0 / 3.0, 1.0, 100.0);
    glm::mat4x4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-1.0f));
    glm::mat4x4 viewMatrix = glm::mat4(1.0f); 
    glm::mat4x4 xRotateMatrix = glm::mat4(1.0f); 
    glm::mat4x4 yRotateMatrix = glm::mat4(1.0f); 
    
    // transformation scaling helper 
    float scaler = 0;

    // Rendering Loop
    shader.activate();
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // settings
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Draw your scene here
        
        printGLError();

        // translation scaling helper 
        int scalerLocation = glGetUniformLocation(shader.get(), "scaler");
        scaler += 0.1;
        glUniform1f(scalerLocation, sin(scaler));

        // MVP
        viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0,-1.0));
        modelMatrix = glm::translate(glm::mat4(1.0f), motionVec);
        yRotateMatrix = glm::rotate(rotationVec[1],glm::vec3(0,1,0));
        xRotateMatrix = glm::rotate(rotationVec[0],glm::vec3(1,0,0));
        MVPmatrix = perspMatrix*yRotateMatrix*xRotateMatrix*viewMatrix*modelMatrix;

        int MVPlocation = glGetUniformLocation(shader.get(), "MVPmatrix");
        glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, glm::value_ptr(MVPmatrix));

        // Draw scene
        glBindVertexArray(index);
        glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
       
        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
    shader.deactivate();
    shader.destroy();
}


void handleKeyboardInput(GLFWwindow* window)
{
    float speed = 0.05;
    // hold space for rotation
    if( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        
        // left/right for yaw
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            rotationVec[1]-= speed;
        }
    
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            rotationVec[1]+= speed;
        }

        // up/down for pitch
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            if (rotationVec[0] < M_PI/2){
                 rotationVec[0]+= speed;
            } 
        }
       
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            if (rotationVec[0] > -M_PI/2){
                 rotationVec[0]-= speed;
            } 


        }
    }

    else {
        // for task 5 a
        glm::mat3x3 yRotateMatrix = glm::rotate(rotationVec[1],glm::vec3(0,1,0));
        glm::mat3x3 xRotateMatrix = glm::rotate(rotationVec[0],glm::vec3(1,0,0));
        
        // left/right for sideways translation
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            glm::vec3 speedVec = glm::vec3(speed,0.0f,0.0f);
            motionVec-=speedVec*yRotateMatrix*xRotateMatrix;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            glm::vec3 speedVec = glm::vec3(speed,0.0f,0.0f);
            motionVec+= speedVec*yRotateMatrix*xRotateMatrix;
        }

        // up/down for up/down 
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            glm::vec3 speedVec = glm::vec3(0.0f,speed,0.0f);
            motionVec-= speedVec*yRotateMatrix*xRotateMatrix;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            glm::vec3 speedVec = glm::vec3(0.0f,speed,0.0f);
            motionVec+= speedVec*yRotateMatrix*xRotateMatrix;
        }

        // W/S for zooming in/out
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            glm::vec3 speedVec = glm::vec3(0.0f,0.0f,speed);
            motionVec+= speedVec*yRotateMatrix*xRotateMatrix;
        }
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            glm::vec3 speedVec = glm::vec3(0.0f,0.0f,speed);
            motionVec-= speedVec*yRotateMatrix*xRotateMatrix;
        }
    }
}


int setUpVAOtriangle(std::vector<float> vertexCoordinates, 
                     std::vector<unsigned int> indexArray,
                     std::vector<float> colorArray)
{
    // setup array
    unsigned int arrayID = 0;
    glGenVertexArrays(1, &arrayID);
    glBindVertexArray(arrayID);
    
    // vertex buffer
    unsigned int vertexBufferID = 0;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexCoordinates.size(), vertexCoordinates.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(0);

    // index buffer
    unsigned int indexBufferID = 1;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexArray.size(), indexArray.data(), GL_STATIC_DRAW);
    
    // color buffer
    unsigned int colorBufferID = 2;
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colorArray.size(), colorArray.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(1);

    return arrayID;
}