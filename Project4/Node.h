#ifndef NODE    // To make sure you don't declare the function more than once by including the header multiple times.
#define NODE
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
class Node
{
public:
	Node();
	~Node();
	virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;
	virtual void draw(GLuint shaderProgram, glm::mat4 C, int drawType) = 0;
	virtual void update() = 0;
};

#endif