#include "Geometry.h"

Geometry::Geometry(OBJObject* object, const char * textureFile)
{
	this->object = object;
	loadTexture(textureFile);
	this->diffuse = glm::vec3(0, 0, 0);
	drawType = 1;
}

Geometry::Geometry(OBJObject* object, glm::vec3 diffuse)
{
	this->object = object;
	this->diffuse = diffuse;
	drawType = 0;
}

Geometry::~Geometry()
{
	delete(object);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 M)
{
	glUniform1i(glGetUniformLocation(shaderProgram, "drawType"), drawType);
	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	if (Window::playerView)
		cameraPos = glm::vec3(Player::cam_pos);
	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, &cameraPos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, &diffuse[0]);
	object->draw(shaderProgram, M);
}

void Geometry::update()
{
}