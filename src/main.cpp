#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/error_report.h"
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/lexer.h"

/**
 * @brief Main entry point for the Windows application.
 *
 * This function initializes the application, reads input from a file,
 * performs lexical analysis, and handles any errors that occur during the process.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance of the application (always NULL for Win32 apps).
 * @param lpCmdLine Command line arguments as a single string.
 * @param nShowCmd Control how the window should be shown.
 * @return int Returns 0 on success, non-zero on failure.
 */
int WINAPI startMainProccess(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    std::cout << "Main Start Proccess reached\n";

    try
    {
        Utils::enableAnsiInConsole();

        const std::string inputFilePath = "C:/coding-projects/CPP-Dev/bassil/input/main.basl";

        reportError(inputFilePath, 5, 10, 14, "Unkown token '=', expected ; at pos:9");

        if (Utils::SetFileAssociationIcon(L".basl", L"C:/coding-projects/CPP-Dev/bassil/assets/img/logo.ico"))
        {
            std::wcout << L"File association icon set successfully!" << std::endl;
        }
        else
        {
            std::wcout << L"Failed to set file association icon." << std::endl;
        }

        // Initialize the application
        Utils::CreateWinAPI32BallonNotification("Started Bassil Lang", "Started Bassil Lang, compiled using G++ from main.cpp using windows.h.", 0);

        // Clear previous logs and lexical analysis output
        Utils::clear_file("C:/coding-projects/CPP-Dev/bassil/output/logs.logs");
        Utils::clear_file("C:/coding-projects/CPP-Dev/bassil/output/after_lex.json");

        // Read input file
        std::string inputContent = Utils::readFileToString(inputFilePath);

        if (inputContent.empty())
        {
            throw std::runtime_error("Input file is empty or could not be read.");
        }

        Utils::general_log("Input string: " + inputContent, true);

        // Perform lexical analysis
        std::vector<Token> tokens = lex(inputContent);

        // Display and save tokens
        display_tokens(tokens);
        save_tokens(tokens, "C:/coding-projects/CPP-Dev/bassil/output/after_lex.json");

        Utils::CreateWinAPI32BallonNotification("Lexical Analysis Complete", "Lexical analysis has been completed successfully.", 0);

        return 0;
    }
    catch (const std::exception &e)
    {
        // Handle any exceptions that occurred during execution
        std::string errorMessage = "An error occurred: " + std::string(e.what());
        Utils::general_log(errorMessage, true);
        Utils::CreateWinAPI32BallonNotification("Error", errorMessage, 1);
        return 1;
    }
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 color;
    void main()
    {
        FragColor = vec4(color, 1.0);
    }
)";

bool isPointInRectangle(double x, double y, float left, float right, float bottom, float top)
{
    return x >= left && x <= right && y >= bottom && y <= top;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Button", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.2f,
        0.5f, -0.2f,
        0.5f, 0.2f,
        -0.5f, 0.2f};
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    bool buttonPressed = false;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        int colorLoc = glGetUniformLocation(shaderProgram, "color");

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convert cursor position to OpenGL coordinates
        float openglX = (2.0f * xpos) / width - 1.0f;
        float openglY = 1.0f - (2.0f * ypos) / height;

        if (isPointInRectangle(openglX, openglY, -0.5f, 0.5f, -0.2f, 0.2f))
        {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                glUniform3f(colorLoc, 0.8f, 0.2f, 0.2f); // Dark red when pressed
                buttonPressed = true;
            }
            else
            {
                glUniform3f(colorLoc, 1.0f, 0.5f, 0.5f); // Light red when hovered
                if (buttonPressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
                {
                    startMainProccess(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
                    buttonPressed = false;
                }
            }
        }
        else
        {
            glUniform3f(colorLoc, 0.5f, 0.5f, 0.5f); // Gray when not interacting
            buttonPressed = false;
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}