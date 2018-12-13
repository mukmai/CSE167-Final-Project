#ifndef TERRAIN
#define TERRAIN
#include "Node.h"
#include "Window.h"
#include "LSystem.h"
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
class Terrain :
	public Node
{
private:
	std::vector<std::vector<glm::vec3>> terrainVertices;
	std::vector<std::vector<glm::vec3>> terrainNormals;
	float distance;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<std::vector<float>> perlinNoiseSeed;
	int nOctaves, stageR, smoothR;
	float scaleBias, heightScale;
	std::list<Node*> childrenList;
	std::set<std::pair<int, int>> usedPositions;
	std::vector<std::pair<Node*, int>> objects;
	glm::vec3 desertColor, grassColor;
	bool spread;
	std::vector<glm::vec3> childrenLocations;

public:
	Terrain(int row, int col, float distance, std::vector<std::pair<Node*, int>> objects);
	~Terrain();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void initializeTerrain();
	void generateVertices();
	void generateNormals();
	void perlinNoise();
	float lerp(float t, float a, float b);
	void setStagePlane();
	void generateObjectPosition(Node* object, int amount);
	void switchSpreading(bool choice);
	glm::vec3 getPosition(glm::vec2 coord);

	GLuint VBO, VBO2, VAO;
	int row, col;
	glm::vec3 center;
	int grassRadius;
};

#endif