// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>
#define nullptr NULL
// Properties
GLuint screenWidth = 800, screenHeight = 600;

float applyTexture = 0.0f,applyLights = 0.0f;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadCubemap(vector<const GLchar*> faces);
GLuint loadTexture(GLchar* path);
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    // Setup and compile our shaders
    Shader shaderMountain("shader.vs", "shader.fs");
    Shader shaderSky("shader_sky.vs", "shader_sky.fs");
    Shader shaderOcean("shader_ocean.vs", "shader_ocean.fs");
    // Load models
    Model Sky((GLchar*)"sky/sky.obj");
    Model Mountain((GLchar*)"mountain/mountain.obj");

    Model Ocean((GLchar*)"ocean/ocean.obj");
    Model OceanRef((GLchar*)"ocean/oceanref.obj");





    // Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the colorbuffer
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // Transformation matrices
        shaderSky.Use();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 2000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniform1f(glGetUniformLocation(shaderSky.Program,"applyT"),applyTexture);
        glUniform1f(glGetUniformLocation(shaderSky.Program,"applyL"),applyLights);
        glUniformMatrix4fv(glGetUniformLocation(shaderSky.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderSky.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        //model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderSky.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        shaderSky.Use();     
        // skybox cube
        Sky.Draw(shaderSky);




        //Moving Ocean and Things a Bit Up
        //model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));

        shaderMountain.Use();
        model = glm::translate(model, glm::vec3(0.0f, -335.0f, 0.0f));
        glUniform1f(glGetUniformLocation(shaderMountain.Program,"iTime"),(GLfloat)glfwGetTime());
        glUniform1f(glGetUniformLocation(shaderMountain.Program,"applyT"),applyTexture);        
        glUniform1f(glGetUniformLocation(shaderMountain.Program,"applyL"),applyLights);        
        glUniformMatrix4fv(glGetUniformLocation(shaderMountain.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderMountain.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderMountain.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Mountain.Draw(shaderMountain);


        shaderOcean.Use(); 


        model = glm::translate(model, glm::vec3(0.0f, 935.0f, 0.0f));
        glUniform1f(glGetUniformLocation(shaderOcean.Program,"iTime"),(GLfloat)glfwGetTime());
        glUniform1f(glGetUniformLocation(shaderOcean.Program,"applyT"),applyTexture);
        glUniform1f(glGetUniformLocation(shaderOcean.Program,"applyL"),applyLights);
        glUniform3f(glGetUniformLocation(shaderOcean.Program,"viewPos"),camera.Position.x,camera.Position.y,camera.Position.z);
        glUniformMatrix4fv(glGetUniformLocation(shaderOcean.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderOcean.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderOcean.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Ocean.Draw(shaderOcean);
        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_T && action == GLFW_PRESS)
        if(applyTexture == 0.0f)
            applyTexture = 1.0f;
        else {
            applyLights = 0.0f;
            applyTexture = 0.0f;
        }

    if(key == GLFW_KEY_L && action == GLFW_PRESS)
        if(applyLights == 0.0f) {
            if(applyTexture == 1.0f)
                applyLights = 1.0f;
        }
        else
        {
            applyLights = 0.0f;
        }

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; 
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}	

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}



#pragma endregion
