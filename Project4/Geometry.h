#ifndef GEOMETRY    // To make sure you don't declare the function more than once by including the header multiple times.
#define GEOMETRY
#include "Node.h"
#include "Window.h"
#include <vector>
#include <algorithm>
#include <iostream>
class Geometry :
	public Node
{
private:
	std::vector<unsigned int> indices_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
public:
	Geometry(const char* filepath, const char* textureFile);
	Geometry(const char* filepath);
	~Geometry();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void draw(GLuint shaderProgram, glm::mat4 C, int drawType);
	void update();
	void parse(const char* filepath);
	void center();
	GLuint VBO, VAO, EBO, VBO2;
	GLuint uProjection, uModel, uView, uDrawType, uCameraPos;
};

#endif