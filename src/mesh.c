// engine/src/mesh.c
#include "engine/mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy and memset
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/obj.h"

// Helper function to configure buffers and attributes
// Assumes interleaved data: Pos(3 floats), Normal(3 floats), TexCoord(2 floats)
void mesh_configure_buffers(struct Mesh* mesh, float* vertex_data, size_t vertex_data_size, unsigned int* index_data, size_t index_count) {
    mesh->vertex_buffer_size = vertex_data_size;
    mesh->index_count = index_count;

    glBindVertexArray(mesh->vao);

    // --- Vertex Buffer Object (VBO) ---
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);

    // --- Element Buffer Object (EBO) ---
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

    // --- Vertex Attributes ---
    // Stride = 8 floats: 3 Pos + 3 Normal + 2 TexCoord
    size_t stride = (3 + 3 + 2) * sizeof(float);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // Offset after Pos and Normal
    glEnableVertexAttribArray(2);

    // Unbind VAO *first*, then VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO after VAO
}


void mesh_init(struct Mesh* mesh) {
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);
    mesh->index_count = 0;
    mesh->vertex_buffer_size = 0;
}

// --- obj_to_mesh Implementation ---
// A simplified implementation using linear search for vertex uniqueness.
// For large models, a hash map would be much more efficient.

typedef struct {
    unsigned int pos_idx;
    unsigned int tex_idx;
    unsigned int norm_idx;
    unsigned int final_idx; // Index in the final vertex buffer
} VertexIndexCombination;

void obj_to_mesh(struct Mesh* mesh, const struct obj* obj) {
    if (!obj || obj->faces_size == 0) {
        fprintf(stderr, "Error: Invalid or empty obj data provided to obj_to_mesh.\n");
        return;
    }

    // Temporary storage for the final vertices and indices
    float* final_vertices = NULL;
    unsigned int* final_indices = NULL;
    size_t final_vertex_count = 0;
    size_t final_index_count = 0;
    size_t final_vertices_capacity = 0; // Start with 0 capacity
    size_t final_indices_capacity = obj->faces_size * 3; // Preallocate indices

    // Storage for tracking unique vertex combinations
    VertexIndexCombination* unique_vertices = NULL;
    size_t unique_vertices_count = 0;
    size_t unique_vertices_capacity = 0;

    final_indices = malloc(final_indices_capacity * sizeof(unsigned int));
    if (!final_indices) {
        fprintf(stderr, "Error: Failed to allocate memory for final indices.\n");
        free(final_vertices);
        free(final_indices);
        free(unique_vertices);
    }

    // Process each face (triangle)
    for (size_t i = 0; i < obj->faces_size; ++i) {
        // Each face has 3 vertices, and each vertex has 3 indices (pos/tex/norm)
        for (int j = 0; j < 3; ++j) {
            unsigned int pos_idx = obj->faces[i * 9 + j * 3 + 0];
            unsigned int tex_idx = obj->faces[i * 9 + j * 3 + 1];
            unsigned int norm_idx = obj->faces[i * 9 + j * 3 + 2];

            // --- Find or add unique vertex ---
            int found_idx = -1;
            for (size_t k = 0; k < unique_vertices_count; ++k) {
                if (unique_vertices[k].pos_idx == pos_idx &&
                    unique_vertices[k].tex_idx == tex_idx &&
                    unique_vertices[k].norm_idx == norm_idx) {
                    found_idx = (int)unique_vertices[k].final_idx;
                    break;
                }
            }

            if (found_idx != -1) {
                // Vertex combination already exists, reuse its index
                final_indices[final_index_count++] = (unsigned int)found_idx;
            } else {
                // New unique vertex combination
                // Check and resize final_vertices if needed
                if (final_vertex_count >= final_vertices_capacity) {
                    final_vertices_capacity = final_vertices_capacity == 0 ? 256 : final_vertices_capacity * 2; // Double capacity
                    float* new_vertices = realloc(final_vertices, final_vertices_capacity * 8 * sizeof(float)); // 8 floats per vertex
                    if (!new_vertices) {
                        fprintf(stderr, "Error: Failed to reallocate final vertex buffer.\n");
                        free(final_vertices);
                        free(final_indices);
                        free(unique_vertices);
                    }
                    final_vertices = new_vertices;
                }

                // Check and resize unique_vertices tracking array if needed
                 if (unique_vertices_count >= unique_vertices_capacity) {
                    unique_vertices_capacity = unique_vertices_capacity == 0 ? 256 : unique_vertices_capacity * 2;
                    VertexIndexCombination* new_unique = realloc(unique_vertices, unique_vertices_capacity * sizeof(VertexIndexCombination));
                     if (!new_unique) {
                        fprintf(stderr, "Error: Failed to reallocate unique vertex tracking array.\n");
                        free(final_vertices);
                        free(final_indices);
                        free(unique_vertices);
                    }
                    unique_vertices = new_unique;
                 }

                // Add the new vertex data (Pos, Norm, Tex)
                size_t current_vertex_offset = final_vertex_count * 8; // 8 floats per vertex

                // Position (3 floats)
                memcpy(final_vertices + current_vertex_offset, obj->vertices + pos_idx * 3, 3 * sizeof(float));
                // Normal (3 floats) - Check if normals exist
                 if (obj->normals && norm_idx < obj->normals_size) {
                    memcpy(final_vertices + current_vertex_offset + 3, obj->normals + norm_idx * 3, 3 * sizeof(float));
                 } else {
                     // Provide default normal if missing (e.g., facing up)
                     final_vertices[current_vertex_offset + 3] = 0.0f;
                     final_vertices[current_vertex_offset + 4] = 1.0f;
                     final_vertices[current_vertex_offset + 5] = 0.0f;
                 }
                // Texture Coordinate (2 floats) - Check if texture coords exist
                if (obj->texture_coords && tex_idx < obj->texture_coords_size) {
                     memcpy(final_vertices + current_vertex_offset + 6, obj->texture_coords + tex_idx * 2, 2 * sizeof(float));
                } else {
                    // Provide default tex coords if missing
                    final_vertices[current_vertex_offset + 6] = 0.0f;
                    final_vertices[current_vertex_offset + 7] = 0.0f;
                }


                // Record this new unique combination
                unique_vertices[unique_vertices_count].pos_idx = pos_idx;
                unique_vertices[unique_vertices_count].tex_idx = tex_idx;
                unique_vertices[unique_vertices_count].norm_idx = norm_idx;
                unique_vertices[unique_vertices_count].final_idx = (unsigned int)final_vertex_count;

                // Add the index of the newly added vertex to the final index list
                final_indices[final_index_count++] = (unsigned int)final_vertex_count;

                final_vertex_count++;
                unique_vertices_count++;
            }
        }
    }

    // Configure the mesh buffers with the generated data
    mesh_configure_buffers(mesh, final_vertices, final_vertex_count * 8 * sizeof(float), final_indices, final_index_count);
}

void mesh_destroy(struct Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    mesh->index_count = 0;
    mesh->vertex_buffer_size = 0;
}