#include "Transform.h"

Transform::Transform(glm::mat4 C)
{
	M = C;
}

Transform::~Transform()
{
	for (auto it = childrenList.begin(); it != childrenList.end(); ++it) {
		delete(*it);
	}
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C) {
	glm::mat4 M_new = C * M;
	for (auto it = childrenList.begin(); it != childrenList.end(); ++it) {
		(*it)->draw(shaderProgram, M_new);
	}	
}

void Transform::update() {

}

void Transform::addChild(Node * child)
{
	childrenList.push_back(child);
}

void Transform::removeChild(Node * child)
{
	childrenList.remove(child);
}

void Transform::rotate(float degree) {
	M = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(1.0f, 0.0f, 0.0f)) * M;
}

void Transform::rotate(glm::mat4 R) {
	M = R * M;
}

void Transform::scale(glm::vec3 scalar) {
	M = glm::scale(M, scalar);
}

void Transform::translate(glm::vec3 pt) {
	M = glm::translate(M, pt);
}
