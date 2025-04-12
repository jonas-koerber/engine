#include "engine/mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/obj.h"

void mesh_init(struct Mesh* mesh) {
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    mesh->vertex_count = 0;
    mesh->index_count = 0;
}

void mesh_set_vertices(struct Mesh* mesh, float* vertices, size_t size) {
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    size_t stride = 6 * sizeof(float); // 3 position + 3 normal

    // Assuming each vertex has 3 floats (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); //
    glEnableVertexAttribArray(1); // 3 normal

    // Unbind the vao and vbo
    // Unbind vao first, ensuring vbo binding is captured correctly by attribute pointers.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mesh->vertex_count = size / stride;
}

void mesh_set_indices(struct Mesh* mesh, unsigned int* indices, size_t count) {
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Unbind the vao and ebo
    // Unbind vao first, ensuring ebo binding is captured correctly by attribute pointers.
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->index_count = count;
}

void obj_to_mesh(struct Mesh* mesh, struct obj* obj) {
    // Set vertices
    float* vertices = malloc(obj->faces_size * sizeof(float));
    unsigned int vertices_count = 0;
    for (size_t i = 0; i < obj->faces_size; i++) {
        vertices[i * 6] = obj->vertices[obj->faces[i]];
        vertices[i * 6 + 1] = obj->vertices[obj->faces[i * 9 + 3]];
        vertices[i * 6 + 2] = obj->vertices[obj->faces[i * 9 + 6]];
        vertices[i * 6 + 3] = obj->normals[obj->faces[i * 9 + 2]];
        vertices[i * 6 + 4] = obj->normals[obj->faces[i * 9 + 5]];
        vertices[i * 6 + 5] = obj->normals[obj->faces[i * 9 + 8]];
        vertices_count += 6;
    }
    mesh_set_vertices(mesh, obj->vertices, vertices_count * sizeof(float));
    free(vertices);

    // Set indices
    unsigned int* indices = malloc(obj->faces_size * 3 * sizeof(unsigned int));
    for (size_t i = 0; i < obj->faces_size; i++) {
        indices[i * 3] = obj->faces[i * 9];
        indices[i * 3 + 1] = obj->faces[i * 9 + 3];
        indices[i * 3 + 2] = obj->faces[i * 9 + 6];
    }
    mesh_set_indices(mesh, indices, obj->faces_size * 3);
    free(indices);
}

void mesh_destroy(struct Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    mesh->vertex_count = 0;
}