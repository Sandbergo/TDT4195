// Local headers
#include "program.hpp"
#include "gloom/shader.hpp"
#include "gloom/gloom.hpp"
#include "OBJLoader.hpp"
#include "sceneGraph.hpp"
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
    // Terrain
    Mesh terrainMesh = loadTerrainMesh("../gloom/resources/lunarsurface.obj");

    std::vector<float> vertexVecTerr = terrainMesh.vertices;
    std::vector<float> colorVecTerr = terrainMesh.colours;
    std::vector<float> normalVecTerr = terrainMesh.normals;
    std::vector<unsigned int> indexVecTerr = terrainMesh.indices;

    int indexTerr = setUpVAOtriangle(vertexVecTerr, colorVecTerr, normalVecTerr, indexVecTerr);

    // Helicopter
    Helicopter heliObj = loadHelicopterModel("../gloom/resources/helicopter.obj");   

    Mesh body = heliObj.body;
    Mesh mainRotor = heliObj.mainRotor;
    Mesh tailRotor = heliObj.tailRotor;
    Mesh door = heliObj.door;

    std::vector<float> vertexVecBody = body.vertices;
    std::vector<float> colorVecBody = body.colours;
    std::vector<float> normalVecBody = body.normals;
    std::vector<unsigned int> indexVecBody = body.indices;
    int indexBody = setUpVAOtriangle(vertexVecBody, colorVecBody, normalVecBody, indexVecBody);

    std::vector<float> vertexVecMainRotor = mainRotor.vertices;
    std::vector<float> colorVecMainRotor = mainRotor.colours;
    std::vector<float> normalVecMainRotor = mainRotor.normals;
    std::vector<unsigned int> indexVecMainRotor = mainRotor.indices;
    int indexMainRotor = setUpVAOtriangle(vertexVecMainRotor, colorVecMainRotor, normalVecMainRotor, indexVecMainRotor);

    
    std::vector<float> vertexVecTailRotor = tailRotor.vertices;
    std::vector<float> colorVecTailRotor = tailRotor.colours;
    std::vector<float> normalVecTailRotor = tailRotor.normals;
    std::vector<unsigned int> indexVecTailRotor = tailRotor.indices;
    int indexTailRotor = setUpVAOtriangle(vertexVecTailRotor, colorVecTailRotor, normalVecTailRotor, indexVecTailRotor);

    std::vector<float> vertexVecDoor = body.vertices;
    std::vector<float> colorVecDoor = body.colours;
    std::vector<float> normalVecDoor = body.normals;
    std::vector<unsigned int> indexVecDoor = body.indices;
    int indexDoor = setUpVAOtriangle(vertexVecDoor, colorVecDoor, normalVecDoor, indexVecDoor);
    
    // transformation matrices
    glm::mat4 MVPmatrix = glm::mat4(1.0f); 
    glm::mat4x4 perspMatrix = glm::perspective(3.14/2.0, 4.0 / 3.0, 1.0, 1000.0);
    glm::mat4x4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-1.0f));
    glm::mat4x4 viewMatrix = glm::mat4(1.0f); 
    glm::mat4x4 xRotateMatrix = glm::mat4(1.0f); 
    glm::mat4x4 yRotateMatrix = glm::mat4(1.0f); 

    SceneNode* root = createSceneGraph();
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

        // MVP
        viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0,-1.0));
		//viewMatrix = glm::mat4(1.0f);
		
		modelMatrix = glm::translate(glm::mat4(1.0f), motionVec);
		xRotateMatrix = glm::rotate(rotationVec[0], glm::vec3(1,0,0));
        yRotateMatrix = glm::rotate(rotationVec[1],glm::vec3(0,1,0));
        MVPmatrix = perspMatrix*xRotateMatrix*yRotateMatrix*viewMatrix*modelMatrix;
        
        int viewLocation = glGetUniformLocation(shader.get(), "viewMatrix");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(MVPmatrix));
		
        // Draw scene
		//updateSceneNode(root, MVPmatrix); // old, works 
		updateSceneNode(root, glm::mat4(1.0f));

        drawSceneNode(root, MVPmatrix, &shader);
       
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
            if (rotationVec[0] < 3.14/2){
                 rotationVec[0]+= speed;
            } 
        }
       
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            if (rotationVec[0] > -3.14/2){
                 rotationVec[0]-= speed;
            } 
        }
    }

    else {
        // for task 5 a
        glm::mat3x3 yRotateMatrix = glm::rotate(rotationVec[1],glm::vec3(0,1,0));
        glm::mat3x3 xRotateMatrix = glm::rotate(rotationVec[0],glm::vec3(1,0,0));
        speed = 0.5;
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


int setUpVAOtriangle(std::vector<float> vertexVec, 
                     std::vector<float> colorVec,
                     std::vector<float> normalVec,
                     std::vector<unsigned int> indexVec)
{
    // setup array
    unsigned int arrayID = 0;
    glGenVertexArrays(1, &arrayID);
    glBindVertexArray(arrayID);
    
    // vertex buffer
    unsigned int vertexBufferID = 0;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexVec.size(), vertexVec.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(0);

    // index buffer
    unsigned int indexBufferID = 1;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexVec.size(), indexVec.data(), GL_STATIC_DRAW);
    
    // color buffer
    unsigned int colorBufferID = 2;
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colorVec.size(), colorVec.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(1);

    // normal buffer
    unsigned int normalBufferID = 3;
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normalVec.size(), normalVec.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT,GL_FALSE,0,0); 
    glEnableVertexAttribArray(2);
    

    return arrayID;
}

SceneNode* createSceneGraph(){
    // Terrain
    Mesh terrainMesh = loadTerrainMesh("../gloom/resources/lunarsurface.obj");

    std::vector<float> vertexVecTerr = terrainMesh.vertices;
    std::vector<float> colorVecTerr = terrainMesh.colours;
    std::vector<float> normalVecTerr = terrainMesh.normals;
    std::vector<unsigned int> indexVecTerr = terrainMesh.indices;
    int indexTerr = setUpVAOtriangle(vertexVecTerr, colorVecTerr, normalVecTerr, indexVecTerr);

    // Helicopter
    Helicopter heliObj = loadHelicopterModel("../gloom/resources/helicopter.obj");   

    Mesh body = heliObj.body;
    Mesh mainRotor = heliObj.mainRotor;
    Mesh tailRotor = heliObj.tailRotor;
    Mesh door = heliObj.door;

    std::vector<float> vertexVecBody = body.vertices;
    std::vector<float> colorVecBody = body.colours;
    std::vector<float> normalVecBody = body.normals;
    std::vector<unsigned int> indexVecBody = body.indices;
    int indexBody = setUpVAOtriangle(vertexVecBody, colorVecBody, normalVecBody, indexVecBody);

    std::vector<float> vertexVecMainRotor = mainRotor.vertices;
    std::vector<float> colorVecMainRotor = mainRotor.colours;
    std::vector<float> normalVecMainRotor = mainRotor.normals;
    std::vector<unsigned int> indexVecMainRotor = mainRotor.indices;
    int indexMainRotor = setUpVAOtriangle(vertexVecMainRotor, colorVecMainRotor, normalVecMainRotor, indexVecMainRotor);
    
    std::vector<float> vertexVecTailRotor = tailRotor.vertices;
    std::vector<float> colorVecTailRotor = tailRotor.colours;
    std::vector<float> normalVecTailRotor = tailRotor.normals;
    std::vector<unsigned int> indexVecTailRotor = tailRotor.indices;
    int indexTailRotor = setUpVAOtriangle(vertexVecTailRotor, colorVecTailRotor, normalVecTailRotor, indexVecTailRotor);

    std::vector<float> vertexVecDoor = body.vertices;
    std::vector<float> colorVecDoor = body.colours;
    std::vector<float> normalVecDoor = body.normals;
    std::vector<unsigned int> indexVecDoor = body.indices;
    int indexDoor = setUpVAOtriangle(vertexVecDoor, colorVecDoor, normalVecDoor, indexVecDoor);


    // Nodes
    SceneNode* terrNode = createSceneNode();
    terrNode->vertexArrayObjectID = indexTerr;
    terrNode->VAOIndexCount = indexVecTerr.size();
    terrNode->referencePoint = glm::vec3(0,0,0);


    SceneNode* bodyNode = createSceneNode();
    bodyNode->vertexArrayObjectID = indexBody;
    bodyNode->VAOIndexCount = indexVecBody.size();
    bodyNode->referencePoint = glm::vec3(0,0,10.0);
	bodyNode->position = glm::vec3(1.0, 1.0, 1.0); // tests


    SceneNode* mainRotorNode = createSceneNode();
    mainRotorNode->vertexArrayObjectID = indexMainRotor;
    mainRotorNode->VAOIndexCount = indexVecMainRotor.size();
    mainRotorNode->referencePoint = glm::vec3(0.0,0.0,10.4);


    SceneNode* tailRotorNode = createSceneNode();
    tailRotorNode->vertexArrayObjectID = indexTailRotor;
    tailRotorNode->VAOIndexCount = indexVecTailRotor.size();
    tailRotorNode->referencePoint = glm::vec3(0.35, 2.3, 10.4);

    SceneNode* doorNode = createSceneNode();
    doorNode->vertexArrayObjectID = indexDoor;
    doorNode->VAOIndexCount = indexVecDoor.size();
    doorNode->referencePoint = glm::vec3(0,0,10.0);

    SceneNode* rootNode = createSceneNode();

    addChild(rootNode,terrNode);
    addChild(terrNode,bodyNode);
    addChild(bodyNode,mainRotorNode);
    addChild(bodyNode,tailRotorNode);
    addChild(bodyNode,doorNode);

    printNode(rootNode);
	printNode(bodyNode);

    return rootNode;
}

void drawSceneNode(SceneNode* root, glm::mat4 viewProjectionMatrix, Gloom::Shader* shader) {

    int vaoID = root->vertexArrayObjectID;


    if (vaoID > -1) {
        int location = glGetUniformLocation(shader->get(), "modelMatrix");
        glBindVertexArray(vaoID);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(root->currentTransformationMatrix)); // new, does not work
        //glUniformMatrix4fv(location, 1, GL_FALSE,glm::value_ptr(viewProjectionMatrix)); // old, works
        glDrawElements(GL_TRIANGLES, root->VAOIndexCount, GL_UNSIGNED_INT, nullptr);
    }

    for(SceneNode* child: root->children) {
        drawSceneNode(child, viewProjectionMatrix, shader);
    }
}


void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar) {
	// Do transformation computations here
	glm::mat4 combinedTransformation = transformationThusFar;
	// Store matrix in the node's currentTransformationMatrix here

	node->currentTransformationMatrix =
		transformationThusFar*
		glm::translate(node->position) *
		glm::translate(node->referencePoint) *
		glm::rotate(node->rotation.x, glm::vec3(1, 0, 0)) *
		glm::rotate(node->rotation.y, glm::vec3(0, 1, 0)) *
		glm::rotate(node->rotation.z, glm::vec3(0, 0, 1)) *
		glm::translate(-node->referencePoint);

	for (SceneNode* child : node->children) {
		updateSceneNode(child, node->currentTransformationMatrix);
	}
}