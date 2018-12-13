#ifndef _TOTEMPART_H_
#define _TOTEMPART_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


#include "Node.h"
#include "Geometry.h"
#include "Transform.h"

class TotemPart :
	public Node
{
public:
	TotemPart(int type, Node* shape);
	TotemPart(TotemPart*); // copy ctor
	~TotemPart();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
		
	// returns the part's related transform (since we'll be needing to set this as a child of a previous part
	// and set its children in Totem's constructor
	Transform* getTransform();

	// set attatchments. Will return -1 if it cannot attatch due to type
	int setTop(TotemPart*);
	int setBottom(TotemPart*);
	int setLeft(TotemPart*);
	int setRight(TotemPart*);
	int setMouth(TotemPart*);

	int getId();

	// link pointers to other totem parts, note that some may not apply
	TotemPart* top;
	TotemPart* bottom;
	TotemPart* left;
	TotemPart* right;
	TotemPart* mouth;

	Node* shape;


private:
	const int BODY_NOWING_NOMOUTH = 0;
	const int BODY_NOWING_MOUTH = 1;
	const int BODY_WING_MOUTH = 2;
	const int BODY_NOWING_NOMOUTH_NOBOTTOM = 3;
	const int BODY_WING_NOMOUTH = 8;
	const int MOUTH = 4;
	const int EARS = 5;
	const int LEFT_WING = 6;
	const int RIGHT_WING = 7;

	// represents geometry and transformation
	Transform* transform;

	// it would be in everyone's interest to create subclasses, but this is the quickest way to accomplish this task
	// 0: body, 1: mouth, 2: left wing, 3: right wing, 4: top, 5: bottom
	int type;


};

#endif