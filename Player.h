#ifndef PLAYER    // To make sure you don't declare the function more than once by including the header multiple times.
#define PLAYER
#include "Node.h"
#include "Transform.h"
#include "PlayerBody.h"
#include "Terrain.h"
#include <algorithm>

class PlayerBody;
class Terrain;

class Player :
	public Node
{
private:
	Transform* posOffset;
	Transform* rotation;
	PlayerBody* playerBody;
	glm::vec2 terrainCoord;
	glm::vec3 position;

	float speed;
public:
	Player(PlayerBody* playerBody, Terrain* terrain);
	~Player();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void move(int dir);
	glm::vec3 cam_backOff;
	glm::mat4 cam_rotateX, cam_rotateY;
	glm::mat4 thirdPersonV;
	Terrain* terrain;
	static glm::vec3 cam_pos;		// e  | Position of camera
	glm::vec3 cam_look_at;	// d  | This is where the camera looks at
	glm::vec3 cam_up;			// up | What orientation "up" is
};

#endif