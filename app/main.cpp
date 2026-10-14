// @formatter:off
#include <glad/glad.h>
// @formatter:on
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

float bgRed = 0.2f;
float bgGreen = 0.3f;
float bgBlue = 0.3f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void update(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); // random brojevi

    // init
    //
    glfwInit();

    // OpenGl 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window\n" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to GLAD initialized" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(bgRed, bgGreen, bgBlue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        update(window);
        glfwSwapBuffers(window);
    }

    // terminate
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (mods & GLFW_MOD_CONTROL)
        {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            int step = 30; // Korak promene veličine u pikselima

            if (key == GLFW_KEY_UP)
                glfwSetWindowSize(window, width, height - step);
            if (key == GLFW_KEY_DOWN)
                glfwSetWindowSize(window, width, height + step);
            if (key == GLFW_KEY_LEFT)
                glfwSetWindowSize(window, width - step, height);
            if (key == GLFW_KEY_RIGHT)
                glfwSetWindowSize(window, width + step, height);
        }
        else
        {
            if (key == GLFW_KEY_R)
            {
                bgRed = 1.0f;
                bgGreen = 0.0f;
                bgBlue = 0.0f;
            }
            if (key == GLFW_KEY_G)
            {
                bgRed = 0.0f;
                bgGreen = 1.0f;
                bgBlue = 0.0f;
            }
            if (key == GLFW_KEY_B)
            {
                bgRed = 0.0f;
                bgGreen = 0.0f;
                bgBlue = 1.0f;
            }
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        bgRed = static_cast<float>(rand()) / RAND_MAX;
        bgGreen = static_cast<float>(rand()) / RAND_MAX;
        bgBlue = static_cast<float>(rand()) / RAND_MAX;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    int stateShift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    int stateShiftRight = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);

    if (stateShift == GLFW_PRESS || stateShiftRight == GLFW_PRESS)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float xNorm = static_cast<float>(xpos) / width;
        float yNorm = static_cast<float>(height - ypos) / height;

        if (xNorm < 0.0f)
            xNorm = 0.0f;

        if (xNorm > 1.0f)
            xNorm = 1.0f;

        if (yNorm < 0.0f)
            yNorm = 0.0f;

        if (yNorm > 1.0f)
            yNorm = 1.0f;

        bgRed = xNorm;
        bgGreen = yNorm;
        bgBlue = 0.0f;
    }
}

void update(GLFWwindow* window) {}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
