#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameState.h"
#include "PlayingState.h"
#include "ResourceManager.h"

int bufferWidth, bufferHeight;
GLFWwindow* mainWindow = nullptr;
GameState* currentState = nullptr;

int createWindow()
{
    if (!glfwInit())
    {
        std::cout << "GLFW initialisation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    ResourceManager::WIDTH = 1920.0f;    ResourceManager::HEIGHT = 1080.0f;
    mainWindow = glfwCreateWindow(ResourceManager::WIDTH, ResourceManager::HEIGHT, "Particles", glfwGetPrimaryMonitor(), NULL);
    if (!mainWindow)
    {
        std::cout << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    return 0;
}

int main()
{
    // Setup window
    if (createWindow())
    {
        return 1;
    }

    // load, compile and link shaders
    ResourceManager::setupShaders();

    // Set current state to menu
    currentState = new PlayingState(mainWindow);

    glUseProgram(ResourceManager::basicShader.getID());

    while (!glfwWindowShouldClose(mainWindow))
    {
        currentState->update();
        currentState->draw();
    }
    
    // Cleanup
    delete currentState;
    glfwTerminate();
    return 0;
}
