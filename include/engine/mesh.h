#pragma once

#include <stddef.h>

struct obj;

struct Mesh {
    unsigned int vao, vbo, ebo;
    size_t  index_count;
    size_t vertex_buffer_size;
};

void mesh_init(struct Mesh* mesh);

void obj_to_mesh(struct Mesh* mesh, const struct obj* obj);

void mesh_destroy(struct Mesh* mesh);