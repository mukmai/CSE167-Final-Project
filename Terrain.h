#ifndef TERRAIN
#define TERRAIN
#include "Node.h"
#include "Window.h"
#include <stdlib.h>
#include <vector>
class Terrain :
	public Node
{
private:
	std::vector<std::vector<glm::vec3>> terrainVertices;
	std::vector<std::vector<glm::vec3>> terrainNormals;
	float distance;
	int row, col;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<std::vector<float>> perlinNoiseSeed;
	int nOctaves, stageR, smoothR;
	float scaleBias, heightScale;


public:
	Terrain(int row, int col, float distance);
	~Terrain();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void initializeTerrain();
	void generateVertices();
	void generateNormals();
	void perlinNoise();
	float lerp(float t, float a, float b);
	void setStagePlane();

	GLuint VBO, VBO2, VAO;
};

#endif