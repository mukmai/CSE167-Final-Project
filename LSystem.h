#ifndef LSYSTEM
#define LSYSTEM
#include "Node.h"
#include "Window.h"
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class LSystem :
	public Node
{
private:
	int type; // type of tree
	float theta; // how much to rotate
	float length, width, angle, widthRatio, lengthRatio, gWidthRatio;
	std::string axiom; // start string
	int depth; // number of iterations to apply rule
	std::vector <std::vector<std::string>> xRules;
	std::vector<glm::vec3> vertices_, lvertices_;
	std::vector<glm::vec3> normals_, lnormals_;
	glm::vec3 woodDiffuse, leafDiffuse;
	std::vector<glm::vec3> leaf;

public:
	LSystem(int type, int depth);
	~LSystem();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void createLeaf(glm::mat4 M); // 'L'
	void expand(); // apply production rule
	void createCylinder(float height, int nEdges, float botWidth, float topWidth, glm::mat4 M);
	GLuint VBO, VBO2, VAO, LVBO, LVBO2, LVAO;
};

#endif