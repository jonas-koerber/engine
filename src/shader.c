#include "engine/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/matrix.h"

void shader_init(struct Shader* shader, const char* vertex_shader, const char* fragment_shader)
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
}

void shader_use(struct Shader* shader)
{
    glUseProgram(shader->program);
}

void shader_set_uniform_mat4(unsigned int location, const Mat4* matrix)
{
    glUniformMatrix4fv(location, 1, GL_TRUE, &matrix->m[0][0]);
}

void shader_destroy(struct Shader* shader)
{
    glDeleteProgram(shader->program);
}