#include "Particle.h"
#include "Window.h"
#include <time.h>

Particle::Particle()
{
	srand(time(NULL));

	// initialize state
	this->position = glm::vec4(-1 * (rand() % 100) - 30, -10.0f, -1 * (rand() % 100) - 30, 0.0f);
	this->velocity = glm::vec3(-1.0f * (rand() % 10), -1.0, -1.0f * (rand() % 10));
	this->life = (rand() % 100) / 100.0;
	this->delta = 0.05f;
}

Particle::~Particle()
{
}

void Particle::draw(GLuint shaderProgram, glm::mat4 C)
{ 
	glPointSize(2.5f);

	// only draw if alive
	if (this->life > 0.0f) {
		glm::vec3 diffuse(0.95f, 0.75f, 0.38f);
		glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, &diffuse[0]);
		glBegin(GL_POINTS);
		glVertex3f(this->position.x, this->position.y, this->position.z);
		glEnd();
	}
}

void Particle::update()
{
	this->life -= this->delta;
	// if alive, update
	if (this->life > 0.0) {
		this->velocity = glm::vec3(this->velocity.x, this->velocity.y - delta*2.0f, this->velocity.z);
		this->position = this->position + this->velocity;
	}
}

void Particle::respawn() {
	this->position = glm::vec4((rand() % 200) + 100, (rand() % 100) - 10, (rand() % 200) + 100, 0.0f);
    this->life = 1.0f;
	this->velocity = glm::vec3(-10 - (rand() % 10), -0.1, -5 - (rand() % 10)); //glm::vec3((1.0 - (rand() % 3)) * (rand() % 10), 0.1f, (1.0 - (rand() % 3)) * (rand() % 10));
}

