#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "engine/display.h"
#include "engine/shader.h"
#include "engine/input.h"
#include "engine/file.h"
#include "engine/mesh.h"

int main(void)
{
    struct File vertexShaderFile = file_read("shader/basic.vert");
    if(vertexShaderFile.data == NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", vertexShaderFile.path);
        return 1;
    }
    const char* vertexShaderSource = vertexShaderFile.data;

    struct File fragmentShaderFile = file_read("shader/basic.frag");
    if(fragmentShaderFile.data == NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", fragmentShaderFile.path);
        return 1;
    }
    const char* fragmentShaderSource = fragmentShaderFile.data;


    struct Display display;
    display_init(&display, 800, 600, "engine");

    struct Shader shader;
    shader_init(&shader, vertexShaderSource, fragmentShaderSource);
    
    file_destroy(&vertexShaderFile);
    file_destroy(&fragmentShaderFile);

    struct Mesh triangle;
    mesh_init(&triangle);

    float vertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f   // top
    };
    mesh_set_vertices(&triangle, vertices, sizeof(vertices));
    int indices[] = {
        0, 1, 2  // Triangle
    };
    mesh_set_indices(&triangle, indices, sizeof(indices) / sizeof(indices[0]));

    // render loop
    while(!display_should_close(&display))
    {
        processInput(display.window);
        
        // draw background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        shader_use(&shader);
        glBindVertexArray(triangle.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle.EBO);
        glDrawElements(GL_TRIANGLES, triangle.indexCount, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
 
        display_update(&display);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    shader_destroy(&shader);

    display_destroy(&display);
    return 0;
}