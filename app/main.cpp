// @formatter:off
#include <glad/glad.h>
// @formatter:on
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include <cmath>
#include <vector>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const char* vertex_shader_source = R"s(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)s";
const char* fragment_shader_source = R"s(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)s";

float bgRed = 0.2f;
float bgGreen = 0.3f;
float bgBlue = 0.3f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void update(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void drawPolygonN(int N, float radius, unsigned int shaderProgram, float width, float height);

int currentN = 0;
bool drawPolygon = false;

int main()
{

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

    unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(bgRed, bgGreen, bgBlue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (drawPolygon && currentN >= 3)
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            drawPolygonN(currentN, 0.5f, shaderProgram, static_cast<float>(width), static_cast<float>(height));
        }

        glfwPollEvents();
        update(window);
        glfwSwapBuffers(window);
    }

    glDeleteProgram(shaderProgram);

    // terminate
    glfwTerminate();
    return 0;
}

void drawPolygonN(int N, float radius, unsigned int shaderProgram, float windowWidth, float windowHeight)
{
    if (N < 3)
        return;

    float aspectRatio = windowWidth / windowHeight;

    std::vector<float> vertices;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= N; ++i)
    {
        float angle = i * (2.0f * static_cast<float>(M_PI) / N);
        float x = (radius * std::cos(angle)) / aspectRatio;
        float y = radius * std::sin(angle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices.size() / 3));

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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

            if (key >= GLFW_KEY_3 && key <= GLFW_KEY_9)
            {
                currentN = key - GLFW_KEY_0;
                drawPolygon = true;
            }
            else if (key >= GLFW_KEY_KP_3 && key <= GLFW_KEY_KP_9)
            {
                currentN = key - GLFW_KEY_KP_0;
                drawPolygon = true;
            }
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
