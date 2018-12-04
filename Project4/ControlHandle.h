#ifndef CONTROLHANDLE    // To make sure you don't declare the function more than once by including the header multiple times.
#define CONTROLHANDLE
#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Window.h"
class ControlHandle :
	public Node
{
private:
	Node* sphere;
	std::vector<glm::vec3> vertices;
public:
	ControlHandle(glm::vec3 offset, Node* sphere);
	~ControlHandle();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void draw(GLuint shaderProgram, glm::mat4 C, int drawType);
	void update();
	GLuint VBO, VAO;
	GLuint uProjection, uModel, uView, uDrawType;
	glm::vec3 offset;
	bool chosen;
};

#endif