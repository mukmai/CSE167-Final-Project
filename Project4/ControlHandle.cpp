#include "ControlHandle.h"

ControlHandle::ControlHandle(glm::vec3 offset, Node* sphere)
{
	chosen = false;
	this->offset = offset;
	this->sphere = sphere;
	update();
}


ControlHandle::~ControlHandle()
{
	delete(sphere);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void ControlHandle::draw(GLuint shaderProgram, glm::mat4 C)
{
	glm::mat4 M1 = glm::translate(glm::mat4(1.0f), offset);
	glm::mat4 M_new1 = C * M1;
	glm::mat4 M2 = glm::translate(glm::mat4(1.0f), -offset);
	glm::mat4 M_new2 = C * M2;
	if (chosen) {
		sphere->draw(shaderProgram, M_new1, 6);
		sphere->draw(shaderProgram, M_new2, 6);
	}
	else {
		sphere->draw(shaderProgram, M_new1, 5);
		sphere->draw(shaderProgram, M_new2, 5);
	}

	// draw line
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	uDrawType = glGetUniformLocation(shaderProgram, "drawType");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &C[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	glUniform1i(uDrawType, 3);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawArrays(GL_LINE_STRIP, 0, (int)vertices.size());
	//glDrawElements(GL_LINE_STRIP, 8, GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void ControlHandle::draw(GLuint shaderProgram, glm::mat4 C, int drawType)
{
	draw(shaderProgram, C);
}

void ControlHandle::update()
{
	vertices.clear();
	vertices.push_back(offset);
	vertices.push_back(-offset);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}