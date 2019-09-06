// Local headers
#include "program.hpp"
#include "gloom/shader.hpp"
#include "gloom/gloom.hpp"
#include <iostream>

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
    std::vector<float> coordinateVec = {
        -0.8, -0.8, 0,      -0.4, -0.8, 0,      0.8, -0.4, 0, 
         
         0.8, 0.8, 0.0,      0.4, 0.8, 0.0,     0.8, 0.4, 0.0, 
         
         0.0, 0.0, 0.0,      1.0, 0.0, 0.0,     0.0, 1.0, 0.0,

        -1.0, 1.0, 0.0,     -1.0, 0.0, 0.0,     0.0, 1.0, 0.0,

         0.0, 0.0, 0.0,     -1.0, 0.0, 0.0,     0.0, -1.0, 0.0,
    };

    std::vector<float> coordinateVec2 = {
        0.6, -0.8, -1.2,     0.0, 0.4, 0,      -0.8, 0.0, 1.2, 
    };
    
    std::vector<unsigned int> indexVec = 
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    int index = setUpVAOtriangle(coordinateVec, indexVec);
    
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        shader.activate();
        printGLError();
        glBindVertexArray(index);
        glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
       
        shader.deactivate();

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
    shader.destroy();
}



void handleKeyboardInput(GLFWwindow* window)
{

    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}


int setUpVAOtriangle(std::vector<float> vertex_coordinates, std::vector<unsigned int> indexArray)
{
    // setup array
    unsigned int arrayID = 0;
    glGenVertexArrays(1, &arrayID);
    glBindVertexArray(arrayID);
    
    // vertex buffer
    unsigned int vertexBufferID = 0;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex_coordinates.size(), vertex_coordinates.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(0);

    // index buffer
    unsigned int indexBufferID = 0;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexArray.size(), indexArray.data(), GL_STATIC_DRAW);

    return arrayID;
}
