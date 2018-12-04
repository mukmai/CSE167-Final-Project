#pragma once
#include "Node.h"
class LightSource :
	public Node
{
private:
	int lightType;
	int on;
	glm::vec3 light_color;
	glm::vec3 light_pos;
	glm::vec3 light_dir;
	float cons_att;

public:
	LightSource(glm::vec3 light_color, glm::vec3 light_dir);
	LightSource(glm::vec3 light_color, glm::vec3 light_pos, float cons_att);
	~LightSource();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
};

