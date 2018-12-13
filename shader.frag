#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;
in vec3 SkyBoxTexCoords;

uniform struct Material {
    vec3 diffuse;
	sampler2D diffuseTex;
} material;

struct DirLight {
	int on;
	vec3 light_color; // color of light
	vec3 light_dir; // direction of light
};

struct PointLight {
	int on;
	vec3 light_color; // color of light
	vec3 light_pos; // position of light
	float cons_att; // k
};

uniform struct Terrain {
	vec3 desert;
	vec3 grass;
	vec3 center;
	int radius;
} terrain;

uniform int drawType; // 0: color; 1: texture; 2: skybox; 3: terrain;
uniform vec3 cameraPos;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform sampler2D tex;
uniform bool toon_shade;

uniform samplerCube skybox;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 CalcDirLight(DirLight light, vec3 normal);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 result = vec3(0,0,0);
	if (drawType == 2) {
		color = texture(skybox, SkyBoxTexCoords);
	}
	else {
//		result += CalcPointLight(pointLight, norm, Position) * pointLight.on;
		result += CalcDirLight(dirLight, norm) * dirLight.on;
		color = vec4(result, 1.0f);
		if (toon_shade) {
			float edge = dot(normalize(cameraPos - Position), Normal);
			edge = max(0, edge);
			if (edge < 0.05) {
				color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos) {
// calculate the vector from this pixels surface to the light source
	vec3 lightDir = normalize(light.light_pos - fragPos);

	// Diffuse shading (n.L)
	float diff = max(dot(normal, lightDir), 0.0f);

	// Attenutation
	float attenuation = 1.0f / (light.cons_att * length(light.light_pos - fragPos));

	vec3 ambient = light.light_color * attenuation;
	vec3 diffuse = light.light_color * diff * attenuation;
	// render with color
	if (drawType == 0) {
		ambient *= material.diffuse;
		diffuse *= material.diffuse;
	}
	// render with texture
	else if (drawType == 1) {
		ambient *= vec3(texture(material.diffuseTex, TexCoords));
		diffuse *= vec3(texture(material.diffuseTex, TexCoords));
	}
	// render for terrain
	else if (drawType == 3) {
		float dist = sqrt(pow(Position.x - terrain.center.x, 2) + pow(Position.z - terrain.center.z, 2));
		if (dist >= terrain.radius) {
			ambient *= terrain.desert;
			diffuse *= terrain.desert;
		} else {
			ambient *= terrain.grass;
			diffuse *= terrain.grass;
		}
	}
	
	return (ambient + diffuse);
}

vec3 CalcDirLight(DirLight light, vec3 normal) {
	vec3 lightDir = normalize(-light.light_dir);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 ambient = light.light_color;
	vec3 diffuse = light.light_color * diff;

	if (toon_shade) {
	    if (diff > 0.95) diffuse = vec3(vec3(1.0) * diffuse);
		else if (diff > 0.5) diffuse = vec3(vec3(0.6) * diffuse);
		else if (diff > 0.25) diffuse = vec3(vec3(0.4) * diffuse);
		else diffuse = vec3(vec3(0.2) * diffuse);
	}

	// render with color
	if (drawType == 0) {
		ambient *= material.diffuse;
		diffuse *= material.diffuse;
	}
	// render with texture
	if (drawType == 1) {
		ambient *= vec3(texture(material.diffuseTex, TexCoords));
		diffuse *= vec3(texture(material.diffuseTex, TexCoords));
	}
	// render for terrain
	else if (drawType == 3) {
		float dist = sqrt(pow(Position.x - terrain.center.x, 2) + pow(Position.z - terrain.center.z, 2));
		if (dist >= terrain.radius) {
			ambient *= terrain.desert;
			diffuse *= terrain.desert;
		} else {
			ambient *= terrain.grass;
			diffuse *= terrain.grass;
		}
	}
	return (ambient + diffuse);
}