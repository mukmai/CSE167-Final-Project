#include "Geometry.h"

Geometry::Geometry(const char * filepath, const char * textureFile)
{
}

Geometry::Geometry(const char * filepath)
{
	parse(filepath);
	center();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// VBO for positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &vertices_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	// VBO for normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), &normals_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);
	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

Geometry::~Geometry()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 M)
{
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	uDrawType = glGetUniformLocation(shaderProgram, "drawType");
	uCameraPos = glGetUniformLocation(shaderProgram, "cameraPos");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniform1i(uDrawType, 0);
	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	glUniform3fv(uCameraPos, 1, &cameraPos[0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (int)indices_.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 M, int drawType)
{
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	uDrawType = glGetUniformLocation(shaderProgram, "drawType");
	uCameraPos = glGetUniformLocation(shaderProgram, "cameraPos");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniform1i(uDrawType, drawType);
	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	glUniform3fv(uCameraPos, 1, &cameraPos[0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (int)indices_.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Geometry::update()
{
}

void Geometry::parse(const char* filepath)
{
	FILE* fp;
	float r, g, b;
	int c1, c2;

	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		std::cerr << "error loading file" << std::endl;
		exit(-1);
	}
	while (1) {
		c1 = fgetc(fp);
		// Check end of file
		if (c1 == EOF) { break; }
		c2 = fgetc(fp);
		// Check if current line is vertex
		if (c1 == 'v' && c2 == ' ') {
			glm::vec3 currV;
			fscanf(fp, "%f %f %f %f %f %f", &currV.x, &currV.y, &currV.z, &r, &g, &b);
			vertices_.push_back(currV);
		}
		// Check if current line is normal
		else if (c1 == 'v' && c2 == 'n') {
			glm::vec3 currVN;
			fscanf(fp, "%f %f %f", &currVN.x, &currVN.y, &currVN.z);
			normals_.push_back(currVN);
		}
		// Check if current line is face
		else if (c1 == 'f' && c2 == ' ') {
			unsigned int i1, i2, i3, v1, v2, v3;
			fscanf(fp, "%d//%d %d//%d %d//%d", &i1, &v1, &i2, &v2, &i3, &v3);

			// Convert 1-based to 0-based
			i1--;
			i2--;
			i3--;
			indices_.push_back(i1);
			indices_.push_back(i2);
			indices_.push_back(i3);
		}
		// Nothing match
		else {
			fscanf(fp, "\n");
		}
	}

	fclose(fp);
	printf("parsed file %s, vertices: %d  normals: %d  indices: %d\n", filepath, (int)vertices_.size(), (int)normals_.size(), (int)indices_.size());

}

void Geometry::center()
{
	float minx, miny, minz, maxx, maxy, maxz, centx, centy, centz;
	minx = vertices_[0].x;
	miny = vertices_[0].y;
	minz = vertices_[0].z;
	maxx = vertices_[0].x;
	maxy = vertices_[0].y;
	maxz = vertices_[0].z;
	// Get max and min of x,y,z in this model
	for (unsigned int i = 0; i < vertices_.size(); ++i) {
		minx = std::min(minx, vertices_[i].x);
		miny = std::min(miny, vertices_[i].y);
		minz = std::min(minz, vertices_[i].z);
		maxx = std::max(maxx, vertices_[i].x);
		maxy = std::max(maxy, vertices_[i].y);
		maxz = std::max(maxz, vertices_[i].z);
	}

	// Get center point
	centx = (minx + maxx) / 2;
	centy = (miny + maxy) / 2;
	centz = (minz + maxz) / 2;
	// Translate model to center
	for (unsigned int i = 0; i < vertices_.size(); ++i) {
		vertices_[i].x -= centx;
		vertices_[i].y -= centy;
		vertices_[i].z -= centz;
	}
}