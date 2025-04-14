
#include "engine/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct File file_read(const char* path)
{
    struct File file = {NULL, 0, path};
    FILE* binary = fopen(path, "rb");

    if(binary == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return file;
    }

    fseek(binary, 0, SEEK_END);
    file.size = ftell(binary);
    fseek(binary, 0, SEEK_SET);

    file.data = (char*)malloc(file.size + 1);
    if(file.data == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for file: %s\n", path);
        fclose(binary);
        file.size = 0;
        return file;
    }

    size_t read = fread(file.data, 1, file.size, binary);
    if(read != file.size)
    {
        fprintf(stderr, "Failed to read file: %s\n", path);
        fclose(binary);
        free(file.data);
        file.size = 0;
        return file;
    }

    file.data[file.size] = '\0';
    fclose(binary);

    return file;
}

void safe_fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
{
    size_t written = fwrite(ptr, size, count, stream);
    if(written != count)
    {
        fprintf(stderr, "Failed to write data to file\n");
        fclose(stream);
        exit(EXIT_FAILURE);
    }
}

void mesh_data_to_file(const char* path, float* vertices, size_t vertex_count, unsigned int* indices, size_t index_count)
{
    FILE* binary = fopen(path, "wb");
    if(binary == NULL)
    {
        fprintf(stderr, "Failed to open file for writing: %s\n", path);
        return;
    }

    uint32_t v_count32 = (uint32_t)vertex_count;
    uint32_t i_count32 = (uint32_t)index_count;

    safe_fwrite("MESH", sizeof(char), 4, binary);
    safe_fwrite(&v_count32, sizeof(uint32_t), 1, binary);
    safe_fwrite(&i_count32, sizeof(uint32_t), 1, binary);
    safe_fwrite(vertices, sizeof(float), vertex_count * 8, binary);
    safe_fwrite(indices, sizeof(unsigned int), index_count, binary);

    fclose(binary);
}

void file_destroy(struct File* file)
{
    free(file->data);
    file->data = NULL;
    file->size = 0;
    file->path = NULL;
}