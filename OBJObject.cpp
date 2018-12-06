#include "OBJObject.h"
#include "Window.h"

OBJObject::OBJObject(const char *filepath, bool sorted)
{
	drawType = 0;
	if (sorted) {
		parseSorted(filepath);
	}
	else {
		parse(filepath);
	}
	center();

	update();

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &VBO3);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	glBufferData(GL_ARRAY_BUFFER, vertices_.size()* sizeof(glm::vec3), &vertices_[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	// We've sent the vertex data over to OpenGL, but there's still something missing.

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

	if (!uvs_.empty()) {
		// VBO for texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, VBO3);
		glBufferData(GL_ARRAY_BUFFER, uvs_.size() * sizeof(glm::vec2), &uvs_[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			2, // This second line tells us how any components there are per vertex. In this case, it's 2 (we have an x, y component)
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
			2 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
			(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	}


	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);
	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath) 
{
	FILE* fp;
	int c1, c2;
	std::vector<unsigned int> vertex_indices_;
	std::vector<unsigned int> normal_indices_;
	std::vector<unsigned int> uv_indices_;
	std::vector<glm::vec3> input_vertices;
	std::vector<glm::vec3> input_normals;
	std::vector<glm::vec2> input_uvs;

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
			fscanf(fp, "%f %f %f\n", &currV.x, &currV.y, &currV.z);
			input_vertices.push_back(currV);
		}
		// Check if current line is normal
		else if (c1 == 'v' && c2 == 'n') {
			glm::vec3 currVN;
			fscanf(fp, "%f %f %f\n", &currVN.x, &currVN.y, &currVN.z);
			input_normals.push_back(currVN);
		}
		// Check if current line is texture coordinate
		else if (c1 == 'v' && c2 == 't') {
			glm::vec2 currVT;
			fscanf(fp, "%f %f\n", &currVT.x, &currVT.y);
			input_uvs.push_back(currVT);
			
		}
		// Check if current line is face
		else if (c1 == 'f' && c2 == ' ') {
			unsigned int v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

			// Convert 1-based to 0-based
			v1--;
			v2--;
			v3--;
			vt1--;
			vt2--;
			vt3--;
			vn1--;
			vn2--;
			vn3--;

			vertex_indices_.push_back(v1);
			vertex_indices_.push_back(v2);
			vertex_indices_.push_back(v3);
			uv_indices_.push_back(vt1);
			uv_indices_.push_back(vt2);
			uv_indices_.push_back(vt3);
			normal_indices_.push_back(vn1);
			normal_indices_.push_back(vn2);
			normal_indices_.push_back(vn3);
		} 
		// Nothing match
		else {
			fscanf(fp, "\n");
		}
	}

	fclose(fp);

	// Assume that you parse indices of v/vt/vn into different std::vector (vertex_indices_, normal_indices_, uv_indices_)
	// input_vertices, input_normals, input_uvs are raw input data from *.obj files
	// vertices_, normals_, uvs_ are aligned data
	for (unsigned i = 0; i < vertex_indices_.size(); i++) {
		vertices_.push_back(input_vertices[vertex_indices_[i]]);
		normals_.push_back(input_normals[normal_indices_[i]]);
		uvs_.push_back(input_uvs[uv_indices_[i]]);
		indices_.push_back(i);
	}

	printf("vertex_indices_: %d  input_vertices: %d  input_normals: %d  input_uvs: %d\n", (int)vertex_indices_.size(), (int)input_vertices.size(), (int)input_normals.size(), (int)input_uvs.size());

	printf("parsed file %s, vertices: %d  normals: %d  uvs: %d  indices: %d\n", filepath, (int)vertices_.size(), (int)normals_.size(), (int)uvs_.size(), (int)indices_.size());

	// Populate the face indices, vertices, texture corrdinates, and normals vectors with the OBJ Object data
}

void OBJObject::parseSorted(const char *filepath)
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

	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObject::draw(GLuint shaderProgram, glm::mat4 M)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &M[0][0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (int)indices_.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void OBJObject::update()
{
	
}

void OBJObject::center()
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