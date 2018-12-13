#include "Splash.h"
#include "Window.h"
#include <iostream>

Splash::Splash(int amount) {
	this->amount = amount;
	this->respawnRate = 100;
	this->particlesOn = true;
	this->iterations = 0;

	for (int i = 0; i < this->amount; i++) {
		// particles is a stand-in for children
		this->particles.push_back(new SplashParticle());
	}
}

Splash::~Splash()
{
	for (int i = 0; i < particles.size(); i++) {
		delete(particles[i]);
	}
}

void Splash::draw(GLuint shaderProgram, glm::mat4 C) {
	// for each particle stored in the vector, draw
	for (int i = 0; i < this->amount; i++) {
		this->particles[i]->draw(shaderProgram, C);
	}
}

void Splash::update() {

	if (this->iterations == 270) {
		std::cout << "turning particles off " << std::endl;
		particlesOn = false;
	}

	if (particlesOn) {
		for (GLuint i = 0; i < this->respawnRate; ++i) {
			int unusedParticle = this->firstUnusedParticle();
			this->particles[unusedParticle]->respawn();
			if (i == respawnRate - 1) {
				this->iterations++;
				std::cout << "iteration: " << i << std::endl;
			}
		}
	}
	// for each particle update its current position
	for (int i = 0; i < this->amount; i++) {
		this->particles[i]->update();
	}
}

GLuint Splash::firstUnusedParticle() {
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

void Splash::toggleParticles() {
	if (particlesOn) {
		turnParticlesOff();
	}
	else {
		turnParticlesOn();
	}
}

void Splash::turnParticlesOff() {
	particlesOn = false;
}

void Splash::turnParticlesOn() {
	iterations = 0;
	particlesOn = true;
}

void Splash::shrinkSplash(float size) {
	for (int i = 0; i < particles.size(); i++) {
		this->particles[i]->setPointSize(size);
	}
}
