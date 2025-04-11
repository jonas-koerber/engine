
#include "engine/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void file_destroy(struct File* file)
{
    free(file->data);
    file->data = NULL;
    file->size = 0;
    file->path = NULL;
}