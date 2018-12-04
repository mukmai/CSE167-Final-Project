#ifndef ANCHORPOINT    // To make sure you don't declare the function more than once by including the header multiple times.
#define ANCHORPOINT
#include "Node.h"
#include "ControlHandle.h"
#include <glm/gtc/matrix_transform.hpp>
class ControlHandle;
class AnchorPoint :
	public Node
{
private:
	Node* sphere;
	ControlHandle* handle;
public:
	glm::vec3 pos;
	bool chosen;
	AnchorPoint(glm::vec3 pos, Node* sphere, ControlHandle* handle);
	~AnchorPoint();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void draw(GLuint shaderProgram, glm::mat4 C, int drawType);
	void update();
	std::vector<glm::vec3> getPoints();
};

#endif