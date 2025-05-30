#pragma once

#include <stddef.h>

struct Mesh {
    unsigned int vao, vbo, ebo;
    size_t  index_count;
    size_t vertex_buffer_size;
};

void mesh_init(struct Mesh* mesh);

void file_to_mesh(struct Mesh* mesh, const char* data, size_t size);

void mesh_destroy(struct Mesh* mesh);