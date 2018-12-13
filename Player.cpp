#include "Player.h"

Player::Player(PlayerBody* playerBody, Terrain* terrain)
{
	this->terrain = terrain;
	this->terrainCoord = glm::vec2(5, 5);
	this->playerBody = playerBody;
	this->posOffset = new Transform(glm::mat4(1.0f));
	this->rotation = new Transform(glm::mat4(1.0f));
	this->posOffset->addChild(rotation);
	this->rotation->addChild(playerBody);
	speed = 0.5f;

	thirdPersonV = glm::mat4(1.0f);
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);		// up | What orientation "up" is
	cam_rotateX = glm::mat4(1.0f);
	cam_rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0, 0, 1));
	cam_backOff = glm::vec3(-10, 4, 0);

	update();
}

Player::~Player()
{
}

void Player::draw(GLuint shaderProgram, glm::mat4 C) {
	posOffset->draw(shaderProgram, C);
}

void Player::update() {
	
	position = terrain->getPosition(terrainCoord);
	posOffset->set(glm::translate(glm::mat4(1.0f), position));
	
	cam_pos =  position + glm::vec3(cam_rotateX * cam_rotateY * glm::vec4(cam_backOff,1));
	cam_look_at = position + glm::vec3(0,3,0);
	thirdPersonV = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

// dir: 0 = forward, 1 = right, 2 = backward, 3 = left
void Player::move(int dir) {
	glm::vec2 direction;
	glm::vec2 camDir = glm::vec2((position - cam_pos).x, (position - cam_pos).z);
	switch (dir) {
	case 0:
		direction = glm::vec2(camDir.x, camDir.y);
		break;
	case 1:
		direction = glm::vec2(-camDir.y, camDir.x);
		break;
	case 2:
		direction = glm::vec2(-camDir.x, -camDir.y);
		break;
	case 3:
		direction = glm::vec2(camDir.y, -camDir.x);
		break;
	}
	terrainCoord += glm::normalize(direction) * speed;
	terrainCoord.x = std::max(terrainCoord.x, 1.0f);
	terrainCoord.x = std::min(terrainCoord.x, (float)terrain->col - 2);
	terrainCoord.y = std::max(terrainCoord.y, 1.0f);
	terrainCoord.y = std::min(terrainCoord.y, (float)terrain->row - 2);

	float angle = atan2(-direction.y, -direction.x);
	angle = (angle + atan2(1,0) * 2) * 360 / (atan2(1, 0) * 4);
	playerBody->move(angle);
	update();
}