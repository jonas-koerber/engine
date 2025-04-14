#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // Accept normal attribute
layout (location = 2) in vec2 aTexCoord; // Accept texture coordinate attribute - not used yet

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;  // Pass position to fragment shader
out vec3 Normal;   // Pass normal to fragment shader

void main()
{
    // Calculate world position and transform normal
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Transform normal to world space (correct for non-uniform scaling)
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}