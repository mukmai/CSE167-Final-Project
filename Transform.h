#ifndef TRANSFORM    // To make sure you don't declare the function more than once by including the header multiple times.
#define TRANSFORM
#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>
#include <list>
class Transform : public Node
{
private:
	std::list<Node*> childrenList;
	glm::mat4 M;

public:
	Transform(glm::mat4 C);
	~Transform();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void addChild(Node* child);
	void removeChild(Node* child);
	void rotate(float degree);
	void rotate(glm::mat4 R);
	void scale(glm::vec3 scalar);
	void translate(glm::vec3 pt);
	void set(glm::mat4 C);
};

#endif