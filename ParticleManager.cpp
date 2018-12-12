#include "ParticleManager.h"
#include "Window.h"
#include <iostream>

ParticleManager::ParticleManager(int amount, Geometry* particleShape) {
	this->amount = amount;

	// initialize a row of particles behind the player
	for (int i = 0; i < this->amount; i++) {
		// particles is a stand-in for children
		this->particles.push_back(new Particle(glm::vec3(0.0f, 0.0f, 0.0f), particleShape));
	}
}

ParticleManager::~ParticleManager() {
	// TODO
}

void ParticleManager::draw(GLuint shaderProgram, glm::mat4 C) {
	// for each particle stored in the vector, draw
	for (int i = 0; i < this->amount; i++) {
		this->particles[i] -> draw(shaderProgram, C);
	}
}

void ParticleManager::update() {
	for (GLuint i = 0; i < 2; ++i) {
		int unusedParticle = this->firstUnusedParticle();
		this->particles[unusedParticle]->respawn();
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