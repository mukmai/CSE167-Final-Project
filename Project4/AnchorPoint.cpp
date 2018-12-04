#include "AnchorPoint.h"

AnchorPoint::AnchorPoint(glm::vec3 pos, Node* sphere, ControlHandle* handle)
{
	chosen = false;
	this->pos = pos;
	this->sphere = sphere;
	this->handle = handle;
}


AnchorPoint::~AnchorPoint()
{
	delete(sphere);
	delete(handle);
}

void AnchorPoint::draw(GLuint shaderProgram, glm::mat4 C) 
{
	glm::mat4 M = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 M_new = C * M;
	if (chosen) {
		sphere->draw(shaderProgram, M_new, 6);
	}
	else {
		sphere->draw(shaderProgram, M_new, 4);
	}
	handle->draw(shaderProgram, M_new);
}

void AnchorPoint::draw(GLuint shaderProgram, glm::mat4 C, int drawType)
{
	draw(shaderProgram, C);
}

void AnchorPoint::update()
{
}

std::vector<glm::vec3> AnchorPoint::getPoints()
{
	std::vector<glm::vec3> res;
	res.push_back(pos - (handle->offset));
	res.push_back(pos);
	res.push_back(pos + (handle->offset));
	return res;
}