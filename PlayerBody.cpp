#include "PlayerBody.h"

PlayerBody::PlayerBody(Geometry* head, Geometry* body)
{
	mainBodyT = new Transform(glm::mat4(1.0f));
	mainBodyR = new Transform(glm::mat4(1.0f));
	Transform* headT = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.5,0.5,0)));



	mainBodyT->addChild(mainBodyR);
	mainBodyR->addChild(headT);
	mainBodyT->addChild(body);
	headT->addChild(head);

}


PlayerBody::~PlayerBody()
{
	delete(mainBodyT);
}

void PlayerBody::draw(GLuint shaderProgram, glm::mat4 C) {
	mainBodyT->draw(shaderProgram, C);
}

void PlayerBody::update() {

}

void PlayerBody::move(float desireAngle) {
	mainBodyR->set(glm::rotate(glm::mat4(1.0f), -glm::radians(desireAngle), glm::vec3(0, 1, 0)));
}