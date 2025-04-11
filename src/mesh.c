#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine/mesh.h"


void mesh_init(struct Mesh* mesh) {
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    mesh->vertexCount = 0;
    mesh->indexCount = 0;
}

void mesh_set_vertices(struct Mesh* mesh, float* vertices, size_t size) {
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // Assuming each vertex has 3 floats (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO
    // Unbind VAO first, ensuring VBO binding is captured correctly by attribute pointers.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mesh->vertexCount = size / (3 * sizeof(float));
}

void mesh_set_indices(struct Mesh* mesh, unsigned int* indices, size_t count) {
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Unbind the VAO and EBO
    // Unbind VAO first, ensuring EBO binding is captured correctly by attribute pointers.
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->indexCount = count;
}

void mesh_destroy(struct Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
    mesh->vertexCount = 0;
}