#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <iostream>
#include <algorithm>
#include "Project3-texture.h"

class OBJObject
{
private:
	std::vector<unsigned int> indices_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec2> uvs_;

	int drawType;
	glm::vec3 diffuse;

public:
	OBJObject(const char* filepath, bool sorted);
	~OBJObject();

	void parse(const char* filepath);
	void parseSorted(const char* filepath);
	void draw(GLuint shaderProgram, glm::mat4 M);
	void update();
	void center();

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, VBO2, VBO3;
	GLuint uLight;
};

#endif