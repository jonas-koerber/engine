#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine/shader.h"

void shader_init(struct Shader *shader, const char *vertex_shader, const char *fragment_shader)
{
    // Vertex shader
    shader->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertex_shader, 1, &vertex_shader, NULL);
    glCompileShader(shader->vertex_shader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader->vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader->vertex_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
    
    // Fragment shader
    shader->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragment_shader, 1, &fragment_shader, NULL);
    glCompileShader(shader->fragment_shader);
    
    // Check for shader compile errors
    glGetShaderiv(shader->fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader->fragment_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // Link shaders
    shader->program = glCreateProgram();
    glAttachShader(shader->program, shader->vertex_shader);
    glAttachShader(shader->program, shader->fragment_shader);
    glLinkProgram(shader->program);

    // Check for linking errors
    glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader->program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    
    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(shader->vertex_shader);
    glDeleteShader(shader->fragment_shader);

    glGenVertexArrays(1, &shader->VAO);
    glGenBuffers(1, &shader->VBO);
}

void shader_bind_vertecies(struct Shader *shader, float vertices[], size_t size, GLenum draw_mode)
{
    shader->draw_mode = draw_mode;
    shader->vertex_size = size;
    shader->vertex_count = size / (3 * sizeof(float));

    glBindVertexArray(shader->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, shader->VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    glBindVertexArray(0);
}

void shader_draw(struct Shader *shader)
{    
    glUseProgram(shader->program);
    glBindVertexArray(shader->VAO);
    glDrawArrays(shader->draw_mode, 0, shader->vertex_count);
    glBindVertexArray(0);
}

void shader_destroy(struct Shader *shader)
{
    glDeleteVertexArrays(1, &shader->VAO);
    glDeleteBuffers(1, &shader->VBO);
    glDeleteProgram(shader->program);
}