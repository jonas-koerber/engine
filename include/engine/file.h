#pragma once

#include <stddef.h>

struct File {
    char* data;
    size_t size;
    const char* path;
};

struct File file_read(const char* path);

void mesh_data_to_file(const char* path, float* vertices, size_t vertex_count, unsigned int* indices, size_t index_count);

void file_destroy(struct File* file);