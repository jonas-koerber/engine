#pragma once

#include <stddef.h>

struct obj {
    float* vertices;
    size_t vertices_size;
    float* texture_coords;
    size_t texture_coords_size;
    float* normals;
    size_t normals_size;
    unsigned int* faces;
    size_t faces_size;
};

void file_to_obj(struct obj* obj, char* data, size_t file_size);