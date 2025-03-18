struct Shader
{
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    unsigned int program;
    unsigned int VAO;
    unsigned int VBO;

    GLenum draw_mode;
    size_t vertex_size;
    size_t vertex_count;
};

void shader_init(struct Shader *shader, const char *vertex_shader_source, const char *fragment_shader_source);

void shader_bind_vertecies(struct Shader *shader, float vertices[], size_t size, GLenum draw_mode);

void shader_draw(struct Shader *shader);

void shader_destroy(struct Shader *shader);