#include "PlayerBody.h"

PlayerBody::PlayerBody(Geometry* head, Geometry* body, Geometry* limb)
{
	float armOutwardAngle = 20.0f;
	mainBodyT = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.4, 0)));
	mainBodyR = new Transform(glm::mat4(1.0f));
	Transform* headT = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0,0.9,0)));
	Transform* headS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)));

	Transform* fatLimb = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(1.3, 1, 1.3)));
	rightArmRout = new Transform(glm::mat4(1.0f));
	Transform* rightArmTout = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.4, 0.4)));
	Transform* rightArmRin = new Transform(glm::rotate(glm::mat4(1.0f), glm::radians(-armOutwardAngle), glm::vec3(1, 0, 0)));
	leftArmRout = new Transform(glm::mat4(1.0f));
	Transform* leftArmTout = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.4, -0.4)));
	Transform* leftArmRin = new Transform(glm::rotate(glm::mat4(1.0f), glm::radians(armOutwardAngle), glm::vec3(1, 0, 0)));
	Transform* armsTin = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, 0)));
	rightLegR = new Transform(glm::mat4(1.0f));
	Transform* rightLegTout = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.2)));
	leftLegR = new Transform(glm::mat4(1.0f));
	Transform* leftLegTout = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.2)));
	Transform* legsTin = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.9, 0)));

	Transform* bodyT = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.08, 0)));
	Transform* bodyS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.375f, 0.475f, 0.405f)));

	
	mainBodyT->addChild(mainBodyR);
	mainBodyR->addChild(headT);
	mainBodyR->addChild(bodyT);
	bodyT->addChild(bodyS);
	bodyS->addChild(body);
	headT->addChild(headS);
	headS->addChild(head);

	bodyT->addChild(rightArmTout);
	rightArmTout->addChild(rightArmRout);
	rightArmRout->addChild(rightArmRin);
	rightArmRin->addChild(armsTin);

	bodyT->addChild(leftArmTout);
	leftArmTout->addChild(leftArmRout);
	leftArmRout->addChild(leftArmRin);
	leftArmRin->addChild(armsTin);

	armsTin->addChild(limb);

	mainBodyR->addChild(rightLegR);
	rightLegR->addChild(rightLegTout);
	rightLegTout->addChild(legsTin);

	mainBodyR->addChild(leftLegR);
	leftLegR->addChild(leftLegTout);
	leftLegTout->addChild(legsTin);

	legsTin->addChild(limb);

	fatLimb->addChild(limb);

	loopTime = 10;
	timer = loopTime / 2;
	dir = 1;
	moving = false;
}


PlayerBody::~PlayerBody()
{
	delete(mainBodyT);
}

void PlayerBody::draw(GLuint shaderProgram, glm::mat4 C) {
	mainBodyT->draw(shaderProgram, C);
}

void PlayerBody::update() {
	if (moving) {
		timer++;
		if (timer == loopTime) {
			timer = 0;
			dir = -dir;
		}
		leftArmRout->rotate(dir * (60.0f / loopTime));
		rightArmRout->rotate(-dir * (60.0f / loopTime));
		leftLegR->rotate(-dir * (45.0f / loopTime));
		rightLegR->rotate(dir * (45.0f / loopTime));
	}
	else {
		leftArmRout->set(glm::mat4(1.0f));
		rightArmRout->set(glm::mat4(1.0f));
		leftLegR->set(glm::mat4(1.0f));
		rightLegR->set(glm::mat4(1.0f));
		timer = loopTime / 2;
	}
}

void PlayerBody::move(float desireAngle) {
	mainBodyR->set(glm::rotate(glm::mat4(1.0f), -glm::radians(desireAngle), glm::vec3(0, 1, 0)));
	moving = true;
}