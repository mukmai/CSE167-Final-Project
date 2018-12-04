#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform samplerCube skybox;
// 0: sphere; 1: skybox; 2: black; 3: yellow; 4: red; 5: green
uniform int drawType;     
uniform vec3 cameraPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    if (drawType == 0) {
	    //vec3 I = normalize(Position - cameraPos);
		//vec3 R = reflect(I, normalize(Normal));
		//color = vec4(texture(skybox, R).rgb, 1.0);
		color = texture(skybox, TexCoords);
	} else if (drawType == 1) {
		color = texture(skybox, TexCoords);
	} else if (drawType == 2) {
		color = vec4(0,0,0,1);
	} else if (drawType == 3) {
		color = vec4(1,1,0,1);
	} else if (drawType == 4) {
		color = vec4(1,0,0,1);
	} else if (drawType == 5) {
		color = vec4(0,1,0,1);
	} else if (drawType == 6) {
		color = vec4(0,0,1,1);
	}
}
