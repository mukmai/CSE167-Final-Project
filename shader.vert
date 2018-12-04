#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 texCoords;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec2 TexCoords;
out vec3 Normal;
out vec3 Position;
out vec3 SkyBoxTexCoords;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * model * vec4(vert, 1.0);
	TexCoords = texCoords;
	Normal = normalize(transpose(inverse(mat3(model))) * vertNormal);
    Position = vec3(model * vec4(vert, 1.0));
	SkyBoxTexCoords = vert;
}
