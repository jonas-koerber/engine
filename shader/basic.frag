#version 330 core
out vec4 FragColor;

in vec3 FragPos; // Receive world position from vertex shader
in vec3 Normal;  // Receive world normal from vertex shader

// Simple fixed light properties (adjust as needed)
uniform vec3 lightPos = vec3(1.2f, 1.0f, 2.0f); // Light position in world space
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 objectColor = vec3(1.0f, 0.5f, 0.31f); // Base object color

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // Intensity based on angle
    vec3 diffuse = diff * lightColor;

    // Final color calculation
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0f);
}