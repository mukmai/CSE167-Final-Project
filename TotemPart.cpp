#include "TotemPart.h"



TotemPart::TotemPart(int type, Node* shape)
{
	this->shape = shape;
	this->type = type;

	this->top = NULL;
	this->bottom = NULL;
	this->left = NULL;
	this->right = NULL;
	this->mouth = NULL;

	this->transform = new Transform(glm::mat4(1.0f));
	this->transform->addChild(shape);
}

TotemPart::TotemPart(TotemPart* toCopy) {
	this->type = toCopy->getId();

	this->top = toCopy->top;
	this->bottom = toCopy->bottom;
	this->left = toCopy->left;
	this->right = toCopy->right;
	this->mouth = toCopy->mouth;

	/*
	this->top = NULL;
	this->bottom = NULL;
	this->left = NULL;
	this->right = NULL;
	this->mouth = NULL;

	*/
	this->transform = new Transform(glm::mat4(1.0f));
	this->transform->addChild(toCopy->shape);
}

TotemPart::~TotemPart()
{
}

int TotemPart::getId() {
	return this->type;
}

int TotemPart::setTop(TotemPart* top) {
	// if part is already set to some pointer, do not replace and return -1 also
	if (this->top)
		return -1;

	if (type == BODY_NOWING_MOUTH || type == BODY_NOWING_NOMOUTH || type == BODY_NOWING_NOMOUTH_NOBOTTOM || type == BODY_WING_MOUTH || type == BODY_WING_NOMOUTH) {
		this->top = top;
		return 0;
	}
	
	return -1;
}

int TotemPart::setBottom(TotemPart* bottom) {
	// if part is already set to some pointer, do not replace and return -1 also
	if (this->bottom)
		return -1;

	if (type == BODY_NOWING_MOUTH || type == BODY_NOWING_NOMOUTH || type == BODY_WING_MOUTH || type == BODY_WING_NOMOUTH) {
		this->bottom = bottom;
		return 0;
	}
	return -1;
}

int TotemPart::setLeft(TotemPart* left) {
	// if part is already set to some pointer, do not replace and return -1 also
	if (this->left)
		return -1;

	if (type == BODY_WING_NOMOUTH || type == BODY_WING_MOUTH) {
		this->left = left;
		return 0;
	}
	return -1;
}

int TotemPart::setRight(TotemPart* right) {
	// if part is already set to some pointer, do not replace and return -1 also
	if (this->right)
		return -1;

	if (type == BODY_WING_MOUTH || type == BODY_WING_NOMOUTH) {
		this->right = right;
		return 0;
	}

	return -1;
}

int TotemPart::setMouth(TotemPart* mouth) {
	std::cout << "mouth status: " << this->mouth << std::endl;

	// if part is already set to some pointer, do not replace and return -1 also
	if (this->mouth || this->mouth != 0)
		return -1;

	if (type == BODY_NOWING_MOUTH || type == BODY_WING_MOUTH) {
		std::cout << "adding a mouth" << this->mouth << std::endl;
		this->mouth = mouth;
		return 0;
	}
}

void TotemPart::draw(GLuint shaderProgram, glm::mat4 C) {
	this->transform->draw(shaderProgram, C);
}

void TotemPart::update() {

}

Transform* TotemPart::getTransform() {
	return this->transform;
}