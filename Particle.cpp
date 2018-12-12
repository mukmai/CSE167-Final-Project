#include "Particle.h"
#include "Window.h"

Particle::Particle(glm::vec3 pos, Geometry* particleShape)
{
	// initialize state
	this->position = glm::vec4((rand() % 100), 30.0f + (rand() % 10), (rand() % 100), 0.0f);
	this->velocity = glm::vec3(-1.0f * (rand() % 10), 0.0f, -1.0f * (rand() % 10));
	this->life = (rand() % 100) / 100.0;
	this->delta = 0.005f;

	//std::cout << "particle initial position: " << this->position.x << ", " << this->position.y << ", " << this->position.z << std::endl;

	// position of the particle
	transform = new Transform(glm::mat4(1.0f));

	// move to back corner of map
	transform->translate(glm::vec3(-10.0f, 0.0f, -10.0f));
	transform->addChild(particleShape);
	transform->scale(glm::vec3(0.01f, 0.01f, 0.01f));
	transform->translate(glm::vec3((rand() % 10), (rand() % 10), (rand() % 10)));
}

Particle::~Particle()
{
	// TODO
}

void Particle::draw(GLuint shaderProgram, glm::mat4 C)
{ 

	// only draw if alive
	if (this->life > 0.0f) {
		//std::cout << "life remaining: " << this->life << std::endl;
		transform->draw(shaderProgram, C);
	}
}

void Particle::update()
{
	this->life -= this->delta;
	//std::cout << "updating life to: " << this->life << std::endl;
	// if alive, update
	if (this->life > 0.0) {
		this->velocity = glm::vec3(this->velocity.x, this->velocity.y - delta*2.0f, this->velocity.z);
		this->transform->translate(this->velocity);
		//this->transform->scale(glm::vec3(1.01f, 1.01f, 1.01f));
	}
}

void Particle::respawn() {
	this->position = glm::vec4((rand() % 100), 30.0f + (rand() % 10), (rand() % 100), 0.0f);
    this->life = 1.0f;
	this->velocity = glm::vec3((1.0 - (rand() % 3)) * (rand() % 10), 0.0f, (1.0 - (rand() % 3)) * (rand() % 10));

	transform->set(glm::mat4(1.0f));
	//transform->translate(glm::vec3(-20.0f, 0.0f, -20.0f));
	transform->translate(glm::vec3((rand() % 10) - 5.0, (rand() % 10), (rand() % 10) - 5.0));
	transform->scale(glm::vec3(0.1f, 0.1f, 0.1f));
}

