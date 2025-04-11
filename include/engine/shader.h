#pragma once

#include "engine/matrix.h"

struct Shader {
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    unsigned int program;
};

void shader_init(struct Shader* shader, const char* vertex_shader_source, const char* fragment_shader_source);

void shader_use(struct Shader* shader);

void shader_set_uniform_mat4(unsigned int location, const Mat4* matrix);

void shader_destroy(struct Shader* shader);