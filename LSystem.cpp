#include "LSystem.h"

LSystem::LSystem(int type, int depth)
{
	leafSwitch = false;
	this->type = type;
	this->depth = depth;
	axiom = "X";

	std::vector<std::string> type1xRules, type2xRules, type3xRules;
	type1xRules.push_back("F[-XL]F[+XL]-X");
	type1xRules.push_back("F[+XL]F[-XL]+X");
	xRules.push_back(type1xRules);
	type2xRules.push_back("F+[[XL]-X]-F[-FXL]+X");
	type2xRules.push_back("F-[[XL]+X]+F[+FXL]-X");
	xRules.push_back(type2xRules);
	type3xRules.push_back("FL[+XL][-XL]FXL");
	type3xRules.push_back("FL[-XL][+XL]FXL");
	xRules.push_back(type3xRules);

	leaf.push_back(glm::vec3(0, 0, 0));
	leaf.push_back(glm::vec3(0.5, 0, 0.7));
	leaf.push_back(glm::vec3(0, 1, 0));
	leaf.push_back(glm::vec3(0, 0, 0));
	leaf.push_back(glm::vec3(-0.5, 0, -0.7));
	leaf.push_back(glm::vec3(0, 1, 0));

	if (type == 0) {
		width = 0.3;
		length = 0.2;
		angle = 25.0;
		widthRatio = 0.75;
		lengthRatio = 1.25;
		gWidthRatio = 0.98;
		woodDiffuse = glm::vec3(0.55, 0.27, 0.07);
		leafDiffuse = glm::vec3(0.50, 0.9, 0.0);
	}
	else if (type == 1) {
		width = 0.3;
		length = 0.2;
		angle = 15.0;
		widthRatio = 0.75;
		lengthRatio = 1.25;
		gWidthRatio = 1;
		woodDiffuse = glm::vec3(0.56, 0.39, 0.02);
		leafDiffuse = glm::vec3(0.0, 0.72, 0.6);
	}
	else if (type == 2) {
		width = 2;
		length = 0.2;
		angle = 35.7;
		widthRatio = 0.35;
		lengthRatio = 0.9;
		gWidthRatio = 0.99;
		woodDiffuse = glm::vec3(0.55, 0.27, 0.07);
		leafDiffuse = glm::vec3(0.12, 0.28, 0.01);
	}

	width *= 0.2 * depth;

	for (int i = 0; i <= depth; i++) {
		expand();
	}

	update();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &vertices_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), &normals_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &LVAO);
	glGenBuffers(1, &LVBO);
	glGenBuffers(1, &LVBO2);
	glBindVertexArray(LVAO);

	glBindBuffer(GL_ARRAY_BUFFER, LVBO);
	glBufferData(GL_ARRAY_BUFFER, lvertices_.size() * sizeof(glm::vec3), &lvertices_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, LVBO2);
	glBufferData(GL_ARRAY_BUFFER, lnormals_.size() * sizeof(glm::vec3), &lnormals_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

LSystem::~LSystem()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
}

void LSystem::createCylinder(float height, int nEdges, float botWidth, float topWidth, glm::mat4 M) {
	float angleStepSize = (float)360 / nEdges;
	float curAngle = 0;
	glm::vec3 normal = glm::normalize(glm::vec3(height, botWidth-topWidth, 0));

	std::vector<glm::vec3> tempV;
	std::vector<glm::vec3> tempN;

	for (int i = 0; i <= nEdges; i++) {
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(curAngle), glm::vec3(0, 1, 0));
		glm::vec3 topV = M * R * glm::vec4(topWidth, height, 0, 1);
		glm::vec3 botV = M * R * glm::vec4(botWidth, 0, 0, 1);
		glm::vec3 curNormal = M * R * glm::vec4(normal,0);
		tempV.push_back(topV);
		tempV.push_back(botV);
		tempN.push_back(curNormal);
		tempN.push_back(curNormal);
		curAngle += angleStepSize;
	}

	for (int i = 3; i < tempV.size(); i += 2) {
		vertices_.push_back(tempV[i - 3]);
		vertices_.push_back(tempV[i - 2]);
		vertices_.push_back(tempV[i - 1]);
		vertices_.push_back(tempV[i - 1]);
		vertices_.push_back(tempV[i - 2]);
		vertices_.push_back(tempV[i]);

		normals_.push_back(tempN[i - 3]);
		normals_.push_back(tempN[i - 2]);
		normals_.push_back(tempN[i - 1]);
		normals_.push_back(tempN[i - 1]);
		normals_.push_back(tempN[i - 2]);
		normals_.push_back(tempN[i]);
	}
}

// Draw the entire tree after the production rule has been applied 'depth' number of times
void LSystem::draw(GLuint shaderProgram, glm::mat4 C) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &C[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "drawType"), 0);
	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	if (Window::playerView)
		cameraPos = glm::vec3(Player::cam_pos);
	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, &cameraPos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, &woodDiffuse[0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, (int)vertices_.size());
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);

	if (leafSwitch) {
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &C[0][0]);
		glUniform1i(glGetUniformLocation(shaderProgram, "drawType"), 0);
		glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, &cameraPos[0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, &leafDiffuse[0]);

		glBindVertexArray(LVAO);
		glDrawArrays(GL_TRIANGLES, 0, (int)lvertices_.size());
		// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
		glBindVertexArray(0);
	}

}

void LSystem::update() {
	glm::mat4 M = glm::mat4(1.0f);
	std::stack<glm::mat4> stack;
	std::stack<float> tstack, bstack;
	float topWidth = width * gWidthRatio;
	float botWidth = width;
	float curLength = length;
	for (int i = 0; i < axiom.length(); i++) {
		char curChar = axiom[i]; // Read the next character in the string

		// Move forward by some length
		if (curChar == 'F' || curChar == 'X') {
			createCylinder(curLength, 8, botWidth, topWidth, M);
			M = glm::translate(M, glm::vec3(0, curLength, 0));
			botWidth = topWidth;
			topWidth *= gWidthRatio;
		}
		// Push current variables onto stack
		else if (curChar == '[') {
			stack.push(M);
			tstack.push(topWidth);
			bstack.push(botWidth);
			botWidth *= widthRatio;
			topWidth *= widthRatio;
			curLength *= lengthRatio;
		}
		// Restore stack variables to previous "["
		else if (curChar == ']') {
			M = stack.top();
			stack.pop();
			topWidth = tstack.top();
			tstack.pop();
			botWidth = bstack.top();
			bstack.pop();
			curLength /= lengthRatio;
		}
		// Draw a leaf
		else if (curChar == 'L') {
			createLeaf(M);
		}
		// Rotate turtle right by some theta
		else if (curChar == '+') {
			if (type == 2) {
				M = glm::rotate(M, -glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			M = glm::rotate(M, -glm::radians(angle * 1.8f), glm::vec3(1.0f, 0.0f, 0.0f));
			M = glm::rotate(M, glm::radians(angle * 4.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		// Rotate turtle left by some theta
		else if (curChar == '-') {
			if (type == 2) {
				M = glm::rotate(M, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			M = glm::rotate(M, glm::radians(angle * 1.8f), glm::vec3(1.0f, 0.0f, 0.0f));
			M = glm::rotate(M, -glm::radians(angle * 4.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

// Add a leaf to the plants 'L'
void LSystem::createLeaf(glm::mat4 M)
{
	for (int i = 0; i < leaf.size(); i++) {
		glm::vec3 curV = M * glm::vec4(leaf[i], 1);
		glm::vec3 curN = M * glm::vec4(leaf[i], 0);
		lvertices_.push_back(curV);
	}
	glm::vec3 n1 = glm::normalize(glm::cross(glm::vec3(0, 1, 0), glm::vec3(-0.5, 0, -0.7)));
	glm::vec3 n2 = glm::normalize(glm::cross(glm::vec3(0, 1, 0), glm::vec3(0.5, 0, 0.7)));
	lnormals_.push_back(n1);
	lnormals_.push_back(n1);
	lnormals_.push_back(n1);
	lnormals_.push_back(n2);
	lnormals_.push_back(n2);
	lnormals_.push_back(n2);
}

// Apply production rule, rule depends on type
void LSystem::expand()
{
	std::string nextChar; // Hold the value of the next char in the L-System string
	// One type of a tree
	int curLen = axiom.length();
	for (int i = curLen - 1; i >= 0; i--) {
		nextChar = axiom[i];

		// Replace variable F with production rule
		if (nextChar.compare("F") == 0) {
			if (rand() % 5 != 0)
				axiom.replace(i, 1, "FF");
		}
		// Replace variable X with production rule
		else if (nextChar.compare("X") == 0) {
			// Do a seed random here to either make branches left or right?
			if (1) {
				// startString.replace(i, 1, "F[-X]F[+X]-X"); // no leaves
				axiom.replace(i, 1, xRules[type][0]); // leaves
			}
			else {
				//LSystem.replace(i, 1, "F[+X]F[-X]+X"); no leaves
				axiom.replace(i, 1, xRules[type][1]); // leaves
			}
		}			
	}
}
