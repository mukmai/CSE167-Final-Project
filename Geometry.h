#ifndef GEOMETRY    // To make sure you don't declare the function more than once by including the header multiple times.
#define GEOMETRY

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include "Node.h"
#include "Window.h"
#include "OBJObject.h"
#include "Project3-texture.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Geometry :
	public Node
{
private:
	OBJObject* object;
	int drawType;
	glm::vec3 diffuse;
public:
	Geometry(OBJObject* object, const char* textureFile);
	Geometry(OBJObject* object, glm::vec3 diffuse);
	~Geometry();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
};

#endif