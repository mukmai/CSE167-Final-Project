#ifndef _PARTICLE_H_
#define _PARTICLE_H_

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
#include "Node.h"
#include "Geometry.h"
#include "Transform.h"

// Forward declarations needed
class Geometry;

class Particle :
	public Node
{
public:
	Particle(glm::vec3 pos, Geometry* particleShape);
	~Particle();

	void draw(GLuint, glm::mat4);
	void update();
	void respawn();

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;

	// particle states
	glm::vec3 position, velocity;
	GLfloat life, delta;
	Transform* transform;
};

#endif

