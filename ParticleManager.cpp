#include "ParticleManager.h"

ParticleManager::ParticleManager(int amount) {
	this->amount = amount;
	this->respawnRate = 10;
	this->particlesOn = true;

	for (int i = 0; i < this->amount; i++) {
		// particles is a stand-in for children
		this->particles.push_back(new Particle());
	}
}

ParticleManager::~ParticleManager() 
{
	for (int i = 0; i < particles.size(); i++) {
		delete(particles[i]);
	}
}

void ParticleManager::draw(GLuint shaderProgram, glm::mat4 C) {
	// for each particle stored in the vector, draw
	for (int i = 0; i < this->amount; i++) {
		this->particles[i] -> draw(shaderProgram, C);
	}
}

void ParticleManager::update() {
	
	if (particlesOn) {
		for (GLuint i = 0; i < this->respawnRate; ++i) {
			int unusedParticle = this->firstUnusedParticle();
			this->particles[unusedParticle]->respawn();
		}
	}
	// for each particle update its current position
	for (int i = 0; i < this->amount; i++) {
		this->particles[i]->update();
	}
}

GLuint ParticleManager::firstUnusedParticle() {
	// linear search from last used particle recorded
	for (GLuint i = this->lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i]->life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	// otherwise search from beginning
	for (int i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i]->life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	// override first if all else fails
	lastUsedParticle = 0;
	return 0;
}

void ParticleManager::toggleParticles() {
	particlesOn = !particlesOn;
}

void ParticleManager::turnParticlesOff() {
	particlesOn = false;
}

void ParticleManager::turnParticlesOn() {
	particlesOn = true;
}