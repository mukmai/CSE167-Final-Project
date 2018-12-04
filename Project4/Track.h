#ifndef TRACK    // To make sure you don't declare the function more than once by including the header multiple times.
#define TRACK
#include "Node.h"
#include "Window.h"
#include "AnchorPoint.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
class AnchorPoint;
class Track :
	public Node
{
private:
	std::vector<glm::vec3> vertices_;
	std::vector<AnchorPoint *> anchorList;
	Node* sphere;
	glm::vec3 pathPos;
	glm::mat4 riderView;
public:
	Track(std::vector<AnchorPoint*> anchorList, Node* sphere);
	~Track();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void draw(GLuint shaderProgram, glm::mat4 C, int drawType);
	void update();
	void addAnchor(AnchorPoint* anchor);
	void sphereMovement();
	glm::mat4 getRiderView();

	GLuint VBO, VAO;
	GLuint uProjection, uModel, uView, uDrawType;
	int startPt;
	float velocity, time, ratio;
	float maxVelocity, minVelocity, maxHeight, minHeight;
	bool moveSphere;
	clock_t begin_time;

};

#endif