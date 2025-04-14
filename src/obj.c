#include "engine/obj.h"
#include "engine/file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_to_obj(struct obj* obj, char* data)
{
    // Initialize the obj structure
    obj->vertices = NULL;
    obj->vertices_size = 0;
    obj->texture_coords = NULL;
    obj->texture_coords_size = 0;
    obj->normals = NULL;
    obj->normals_size = 0;
    obj->faces = NULL;
    obj->faces_size = 0;

    // Parse the OBJ file data
    char* line = strtok(data, "\n");
    while (line != NULL) {
        if (strncmp(line, "v ", 2) == 0) {
            // Vertex position
            obj->vertices_size++;
            // Allocate memory for vertices
            obj->vertices = realloc(obj->vertices, obj->vertices_size * 3 * sizeof(float));
            if (obj->vertices == NULL) {
                fprintf(stderr, "Memory allocation failed for vertices.\n");
                return;
            }
            // Parse vertex position
            float x, y, z;
            sscanf(line + 2, "%f %f %f", &x, &y, &z);
            obj->vertices[(obj->vertices_size - 1) * 3] = x;
            obj->vertices[(obj->vertices_size - 1) * 3 + 1] = y;
            obj->vertices[(obj->vertices_size - 1) * 3 + 2] = z;
        } else if(strncmp(line, "vt ", 3) == 0) {
            // Texture coordinate
            obj->texture_coords_size++;
            // Allocate memory for texture coordinates
            obj->texture_coords = realloc(obj->texture_coords, obj->texture_coords_size * 2 * sizeof(float));
            if (obj->texture_coords == NULL) {
                fprintf(stderr, "Memory allocation failed for texture coordinates.\n");
                return;
            }
            // Parse texture coordinate
            float u, v;
            sscanf(line + 3, "%f %f", &u, &v);
            obj->texture_coords[(obj->texture_coords_size - 1) * 2] = u;
            obj->texture_coords[(obj->texture_coords_size - 1) * 2 + 1] = v;
        } else if(strncmp(line, "vn ", 3) == 0) {
            // Vertex normal
            obj->normals_size++;
            // Allocate memory for normals
            obj->normals = realloc(obj->normals, obj->normals_size * 3 * sizeof(float));
            if (obj->normals == NULL) {
                fprintf(stderr, "Memory allocation failed for normals.\n");
                return;
            }
            // Parse vertex normal
            float nx, ny, nz;
            sscanf(line + 3, "%f %f %f", &nx, &ny, &nz);
            obj->normals[(obj->normals_size - 1) * 3] = nx;
            obj->normals[(obj->normals_size - 1) * 3 + 1] = ny;
            obj->normals[(obj->normals_size - 1) * 3 + 2] = nz;
        } else if(strncmp(line, "f ", 2) == 0) {
            // Face
            obj->faces_size++;
            // Allocate memory for faces
            obj->faces = realloc(obj->faces, obj->faces_size * 9 * sizeof(unsigned int));
            if (obj->faces == NULL) {
                fprintf(stderr, "Memory allocation failed for faces.\n");
                return;
            }
            // Parse face
            unsigned int v1, v2, v3;
            unsigned int vt1, vt2, vt3;
            unsigned int vn1, vn2, vn3;
            unsigned int assigned;
            assigned = sscanf(line + 2, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
            if(assigned != 9) {
                assigned = sscanf(line + 2, "%u//%u %u//%u %u//%u", &v1, &vn1, &v2, &vn2, &v3, &vn3);
                if(assigned != 6) {
                    assigned = sscanf(line + 2, "%u %u %u", &v1, &v2, &v3);
                    if(assigned != 3) {
                        assigned = sscanf(line + 2, "%u/%u %u/%u %u/%u", &v1, &vt1, &v2, &vt2, &v3, &vt3);
                        if(assigned != 6) {
                            fprintf(stderr, "Failed to parse face: %s\n", line);
                            free(obj->faces);
                            obj->faces = NULL;
                            obj->faces_size = 0;
                            return;
                        }
                    }
                }
            }

            // Store face indices
            obj->faces[(obj->faces_size - 1) * 9] = v1 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 1] = vt1 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 2] = vn1 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 3] = v2 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 4] = vt2 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 5] = vn2 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 6] = v3 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 7] = vt3 - 1; // OBJ indices are 1-based
            obj->faces[(obj->faces_size - 1) * 9 + 8] = vn3 - 1; // OBJ indices are 1-based
        }
        
        line = strtok(NULL, "\n");
    }
}