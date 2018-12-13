#include "SplashParticle.h"
#include "Window.h"
#include <time.h>

SplashParticle::SplashParticle()
{
	srand(Window::seed);

	// initialize state
	this->position = glm::vec3(0, 100, 0);
	this->velocity = glm::vec3((5 - (rand() % 10))/10, +1.5, (5 - (rand() % 10)) / 10);
	this->life = (rand() % 100) / 100.0;
	this->delta = 0.005f;
	this->pointSize = 5.5f;
}

SplashParticle::~SplashParticle()
{
}

void SplashParticle::draw(GLuint shaderProgram, glm::mat4 C)
{
	//srand(time(NULL));
	//glPointSize(rand() % 7);
	glPointSize(pointSize);

	// only draw if alive
	if (this->life > 0.0f) {
		glm::vec3 diffuse(102 /255.0, 204 /255.0, 255 /255.0);
		glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, &diffuse[0]);
		glBegin(GL_POINTS);
		glVertex3f(this->position.x, this->position.y, this->position.z);
		glEnd();
	}
}

void SplashParticle::update()
{
	glPointSize(pointSize);

	this->life -= this->delta;
	// if alive, update
	if (this->life > 0.0) {
		this->velocity = glm::vec3(this->velocity.x, this->velocity.y - delta * 100.0f, this->velocity.z);
		this->position = this->position + this->velocity;
	}
}

void SplashParticle::respawn() {
	this->position = glm::vec3(0, 100, 0);
	this->life = 1.0f;
	//this->velocity = glm::vec3(-5 - (rand() % 5), +1.5, -2.5 - (rand() % 5));
	this->velocity = glm::vec3((5 - (rand() % 10))/4, (rand() % 3), (5 - (rand() % 10))/4);
}

void SplashParticle::setPointSize(float size) {
	this->pointSize = size;
}