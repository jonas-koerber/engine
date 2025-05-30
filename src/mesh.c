// engine/src/mesh.c
#include "engine/mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy and memset
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/file.h"

// Helper function to configure buffers and attributes
// Assumes interleaved data: Pos(3 floats), Normal(3 floats), TexCoord(2 floats)
void mesh_configure_buffers(struct Mesh* mesh, float* vertex_data, size_t vertex_data_size, unsigned int* index_data, size_t index_count) {
    mesh->vertex_buffer_size = vertex_data_size;
    mesh->index_count = index_count;

    glBindVertexArray(mesh->vao);

    // --- Vertex Buffer Object (VBO) ---
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);

    // --- Element Buffer Object (EBO) ---
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

    // --- Vertex Attributes ---
    // Stride = 8 floats: 3 Pos + 3 Normal + 2 TexCoord
    size_t stride = (3 + 3 + 2) * sizeof(float);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // Offset after Pos and Normal
    glEnableVertexAttribArray(2);

    // Unbind VAO *first*, then VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO after VAO
}


void mesh_init(struct Mesh* mesh) {
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);
    mesh->index_count = 0;
    mesh->vertex_buffer_size = 0;
}

void file_to_mesh(struct Mesh* mesh, const char* data, size_t size) {
    if (size < 4 || strncmp(data, "MESH", 4) != 0) {
        fprintf(stderr, "Error: Invalid mesh file format.\n");
        return;
    }

    uint32_t vertex_count = *(uint32_t*)(data + 4);
    uint32_t index_count = *(uint32_t*)(data + 8);

    // Calculate the size of the vertex data
    size_t vertex_data_size = vertex_count * 8 * sizeof(float); // 8 floats per vertex

    // Allocate memory for vertices and indices
    float* vertices = malloc(vertex_data_size);
    unsigned int* indices = malloc(index_count * sizeof(unsigned int));
    if (!vertices || !indices) {
        fprintf(stderr, "Error: Failed to allocate memory for mesh data.\n");
        free(vertices);
        free(indices);
        return;
    }

    // Copy the vertex and index data from the file
    memcpy(vertices, data + 12, vertex_data_size);
    memcpy(indices, data + 12 + vertex_data_size, index_count * sizeof(unsigned int));

    // Configure the mesh buffers with the loaded data
    mesh_configure_buffers(mesh, vertices, vertex_data_size, indices, index_count);

    // Clean up
    free(vertices);
    free(indices);
}

void mesh_destroy(struct Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    mesh->index_count = 0;
    mesh->vertex_buffer_size = 0;
}