#ifndef _SPLASH_H_
#define _SPLASH_H_

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
#include "SplashParticle.h"

// Forward declarations needed
class Particle;
class Node;
class SplashParticle;

class Splash :
	public Node
{
private:
	GLuint firstUnusedParticle();
	GLuint amount;
	GLuint lastUsedParticle = 0;

	std::vector<SplashParticle*> particles;

	int iterations;
	int respawnRate;
	bool particlesOn;

public:
	Splash(int amount);
	~Splash();

	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void toggleParticles();
	void turnParticlesOff();
	void turnParticlesOn();
	void shrinkSplash(float size);


};

#endif

