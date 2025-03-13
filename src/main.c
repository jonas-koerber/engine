#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "engine/display.h"
#include "engine/shader.h"
#include "engine/input.h"

// Vertex Shader source code
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment Shader source code
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main(void)
{
    struct Display display;
    display_init(&display, 800, 600, "engine");

    struct Shader shader;
    shader_init(&shader, vertexShaderSource, fragmentShaderSource);

    // render loop
    while(!display_should_close(&display))
    {
        processInput(display.window);
        
        // draw background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our triangle
        glUseProgram(shader.program);
        glBindVertexArray(shader.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        display_update(&display);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    shader_destroy(&shader);

    display_destroy(&display);
    return 0;
}