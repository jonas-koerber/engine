#pragma once

#include <stddef.h>

struct File {
    char* data;
    size_t size;
    const char* path;
};

struct File file_read(const char* path);

void file_destroy(struct File* file);