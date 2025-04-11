#pragma once

struct Mesh {
    unsigned int vao, vbo, ebo;
    unsigned int vertex_count;
    unsigned int index_count;
};

void mesh_init(struct Mesh* mesh);

void mesh_set_vertices(struct Mesh* mesh, float* vertices, size_t size);

void mesh_set_indices(struct Mesh* mesh, unsigned int* indices, size_t count);

void mesh_destroy(struct Mesh* mesh);