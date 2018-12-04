#ifndef CUBEMAP    // To make sure you don't declare the function more than once by including the header multiple times.
#define CUBEMAP
#include "Node.h"
#include "Project3-texture.h"
#include <vector>
#include "Window.h"
class CubeMap :
	public Node
{
private:
	std::vector<glm::vec3> vertices_;
	glm::vec3 mode;
	unsigned int cubemapTexture;
	unsigned int skyboxVAO, skyboxVBO;
	GLuint uProjection, uModel, uView, uSkybox, uDrawType;

public:
	CubeMap(std::vector<const char*> textureFiles);
	~CubeMap();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void draw(GLuint shaderProgram, glm::mat4 C, int drawType);
	void update();

};

#endif