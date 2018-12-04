#include "Track.h"

Track::Track(std::vector<AnchorPoint*> anchorList, Node* sphere)
{
	this->anchorList = anchorList;
	this->sphere = sphere;
	update();
	startPt = 0;
	ratio = 1;
	velocity = 1;
	moveSphere = true;
	sphereMovement();
	riderView = Window::V;
}


Track::~Track()
{
	for (auto it = anchorList.begin(); it != anchorList.end(); ++it) {
		delete(*it);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Track::draw(GLuint shaderProgram, glm::mat4 C)
{
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	uDrawType = glGetUniformLocation(shaderProgram, "drawType");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &C[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniform1i(uDrawType, 2);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawArrays(GL_LINE_STRIP, 0, (int)vertices_.size());
	//glDrawElements(GL_LINE_STRIP, 8, GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);

	for (auto it = anchorList.begin(); it != anchorList.end(); ++it) {
		(*it)->draw(shaderProgram, C);
	}

	sphereMovement();

	glm::mat4 M = glm::translate(glm::mat4(1.0f), pathPos);
	glm::mat4 M_new = C * M;
	sphere->draw(shaderProgram, M_new);

	glm::vec3 riderPos = glm::vec3(C * glm::translate(glm::mat4(1.0f), pathPos)[3]) + glm::vec3(0,0.2,0);
	//glm::vec3 riderLookAt = riderPos - vertices_[startPt + 1] - pathPos;
	glm::vec3 temp = vertices_[(startPt + 4) % vertices_.size()];
	glm::vec3 riderLookAt = glm::vec3(C * glm::translate(glm::mat4(1.0f), temp)[3]) + glm::vec3(0, 0.2, 0);
	riderView = C * glm::lookAt(riderPos, riderLookAt, glm::vec3(0.0f, 2.0f, 0.0f));
}

void Track::draw(GLuint shaderProgram, glm::mat4 M, int drawType)
{
	draw(shaderProgram, M);
}

void Track::update()
{
	vertices_.clear();

	std::vector<glm::vec3> controlPoints;
	for (auto it = anchorList.begin(); it != anchorList.end(); ++it) {
		std::vector<glm::vec3> dummy = (*it)->getPoints();
		for (int i = 0; i < dummy.size(); i++) {
			controlPoints.push_back(dummy[i]);
		}
	}
	controlPoints.push_back(controlPoints[0]);
	controlPoints.push_back(controlPoints[1]);
	controlPoints.erase(controlPoints.begin());

	for (int i = 0; i < controlPoints.size()-2; i += 3) {
		glm::vec3 p0 = controlPoints[i];
		glm::vec3 p1 = controlPoints[i+1];
		glm::vec3 p2 = controlPoints[i+2];
		glm::vec3 p3 = controlPoints[i+3];
		glm::vec3 a = -p0 + 3.0f*p1 - 3.0f*p2 + p3;
		glm::vec3 b = 3.0f*p0 - 6.0f*p1 + 3.0f*p2;
		glm::vec3 c = -3.0f*p0 + 3.0f*p1;

		vertices_.push_back(p0);
		for (float f = 1; f < 150; f++) {
			float t = f / 150;
			vertices_.push_back(a*t*t*t + b * t*t + c * t + p0);
		}
		vertices_.push_back(p3);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &vertices_[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	maxHeight = vertices_[0].y;
	minHeight = vertices_[0].y;
	for (int i = 0; i < vertices_.size(); i++) {
		if (vertices_[i].y > maxHeight) maxHeight = vertices_[i].y;
		if (vertices_[i].y < minHeight) minHeight = vertices_[i].y;
	}
}

void Track::addAnchor(AnchorPoint* anchor)
{
	anchorList.push_back(anchor);
}

void Track::sphereMovement() {
	time = float(clock() - begin_time) / CLOCKS_PER_SEC;

	if (moveSphere) {
		float curHeightRatio = (pathPos.y-minHeight)/(maxHeight-minHeight);
		velocity = minVelocity + (1 - curHeightRatio) * (maxVelocity - minVelocity);
		printf("%f\n", velocity);
		float distance = velocity * time;
		// ratio always between 0-1 (end to curPos)
		while (distance >= glm::length(ratio * (vertices_[startPt + 1] - vertices_[startPt]))) {
			distance -= glm::length(ratio * (vertices_[startPt + 1] - vertices_[startPt]));
			startPt++;
			if (startPt >= vertices_.size() - 1) startPt = 0;
			ratio = 1;
		}
		ratio -= distance / glm::length(vertices_[startPt + 1] - vertices_[startPt]);
	}

	pathPos = vertices_[startPt + 1] + ratio * (vertices_[startPt + 1] - vertices_[startPt + 1]);
	begin_time = clock();
}

glm::mat4 Track::getRiderView() {
	return riderView;
}