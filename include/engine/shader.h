// shader loading and compiling

struct Shader
{
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    unsigned int program;
    unsigned int VAO;
    unsigned int VBO;
};

void shader_init(struct Shader *shader, const char *vertex_shader_source, const char *fragment_shader_source);

void shader_destroy(struct Shader *shader);