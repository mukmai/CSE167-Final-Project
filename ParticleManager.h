#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

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
#include "Particle.h"

// Forward declarations needed
class Particle;

class ParticleManager :
	public Node
{
private:
	GLuint firstUnusedParticle();
	GLuint shaderProgram, amount;
	GLuint lastUsedParticle = 0;

	std::vector<Particle*> particles;

public:
	ParticleManager(int amount, Geometry* particleShape);
	~ParticleManager();

	glm::mat4 toWorld;

	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();

};

#endif

