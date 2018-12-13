#ifndef _TOTEM_H_
#define _TOTEM_H_

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

#include "TotemPart.h"
#include "Transform.h"
#include "Node.h"

class Node;
class TotemPart;

class Totem :
	public Node
{
public:
	Totem(std::vector<TotemPart*> totemParts, int counterVal);
	~Totem();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();

	Transform* getTransform();

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
	

	// contains base transform for the bottom of the totem
	Transform* transform;

	float rotateVal;
	int runningCount;

	void generateTotem(int counterVal);
	void createCategories(std::vector<TotemPart*> totemParts);

	TotemPart* Totem::getRandomTop();
	TotemPart* Totem::getRandomBottom();
	TotemPart* Totem::getRandomLeft();
	TotemPart* Totem::getRandomRight();
	TotemPart* Totem::getRandomMouth();

	// vector containing each part of the final totem
	std::vector<TotemPart*> parts;
	// each base component type gets its own vector
	// note that each label means that when a body piece is looking for something to attatch at that label, that
	// object will be able to fulfil the request. Eg. if looking for a top piece, we can select an Ear even though
	// Ears themselves only have a Bottom label.
	std::vector<TotemPart*> top, bottom, left, right, mouth;
	// another categorization scheme
	std::vector<TotemPart*> bodies, ears, rightWings, leftWings, mouths;
};

#endif