#ifndef PLAYERBODY    // To make sure you don't declare the function more than once by including the header multiple times.
#define PLAYERBODY
#include "Node.h"
#include "Transform.h"
#include "Geometry.h"

class Geometry;

class PlayerBody :
	public Node
{
private:
	Transform* mainBodyT;
	Transform* mainBodyR;
	Transform* leftLegR;
	Transform* rightLegR;

public:
	PlayerBody(Geometry* head, Geometry* body);
	~PlayerBody();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void move(float desireAngle);
	float curAngle;
};

#endif