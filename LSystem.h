#ifndef LSYSTEM_H
#define LSYSTEM_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
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
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <time.h>

using namespace std;

class LSystem
{

private:
	int type; // type of tree
	float length; // length to draw forward
	float theta; // how much to rotate
	string axiom; // start string
	int lineWidth = 5; // thickness of branches
	int depth; // number of iterations to apply rule
	string tree; // tree to draw
	vector<pair <string,int> > trees; // list of trees to draw


public:
	LSystem();
	void initializeTrees();
	void push(); // '['
	void pop(); // ']'
	void rotateLeft(float theta); // '+'
	void rotateRight(float theta); // '-'
	void addLeaf(); // 'L'
	void moveForward(float length); // 'F'
	void draw(); // draw tree after fully expanded rule
	string expand(int type, string startString); // apply production rule
	void update(); // make plants blow in the wind
};

#endif