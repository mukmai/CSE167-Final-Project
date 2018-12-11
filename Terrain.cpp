#include "Terrain.h"



Terrain::Terrain(int row, int col, float distance, std::vector<std::pair<Node*,int>> objects)
{
	this->distance = distance;
	this->row = row;
	this->col = col;
	this->objects = objects;
	nOctaves = 5;
	scaleBias = 1.5;
	heightScale = 90;
	stageR = 5;
	smoothR = 50;
	desertColor = glm::vec3(0.96, 0.64, 0.38);
	grassColor = glm::vec3(0.53, 0.78, 0.38);
	grassRadius = -1;
	spread = false;
	
	initializeTerrain();

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
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	for (auto it = childrenList.begin(); it != childrenList.end(); ++it) {
		delete(*it);
	}
}

void Terrain::draw(GLuint shaderProgram, glm::mat4 C) {
	if (spread && grassRadius < row) {
		grassRadius++;
	}
	else if (!spread && grassRadius > -1) {
		grassRadius--;
	}
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &C[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "drawType"), 3);
	glm::vec3 cameraPos = glm::vec3(Window::V[3]);
	glm::vec3 diffuse = glm::vec3(0.96,0.64,0.38);
	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, &cameraPos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "terrain.desert"), 1, &desertColor[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "terrain.grass"), 1, &grassColor[0]);
	glm::vec3 center = glm::vec4(terrainVertices[row / 2][col / 2], 1);
	glUniform3fv(glGetUniformLocation(shaderProgram, "terrain.center"), 1, &center[0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain.radius"), grassRadius);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)vertices_.size());
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	for (auto it = childrenList.begin(); it != childrenList.end(); ++it) {
		(*it)->draw(shaderProgram, C);
	}
}

void Terrain::update() {
	vertices_.clear();
	normals_.clear();
	for (int i = 1; i < row; i++) {
		vertices_.push_back(terrainVertices[i - 1][0]);
		normals_.push_back(terrainNormals[i - 1][0]);
		for (int j = 0; j < col; j++) {
			vertices_.push_back(terrainVertices[i - 1][j]);
			vertices_.push_back(terrainVertices[i][j]);
			normals_.push_back(terrainNormals[i - 1][j]);
			normals_.push_back(terrainNormals[i][j]);
		}
		vertices_.push_back(terrainVertices[i][col-1]);
		normals_.push_back(terrainNormals[i][col - 1]);
	}
}

void Terrain::initializeTerrain() {
	perlinNoise();
	setStagePlane();
	generateVertices();
	generateNormals();
	for (auto& object : objects) {
		generateObjectPosition(object.first, object.second);
	}
	update();
}

void Terrain::generateVertices() {
	terrainVertices.clear();
	float rowPos = (float)(distance * row) / 2 - distance * row;
	for (int i = 0; i < row; i++) {
		float colPos = (float)(distance * col) / 2 - distance * col;
		std::vector<glm::vec3> curRow;
		for (int j = 0; j < col; j++) {
			curRow.push_back(glm::vec3(rowPos, perlinNoiseSeed[i][j], colPos));
			colPos += distance;
		}
		terrainVertices.push_back(curRow);
		rowPos += distance;
	}

}

void Terrain::generateNormals() {
	terrainNormals.clear();
	for (int i = 0; i < row; i++) {
		std::vector<glm::vec3> curRow;
		for (int j = 0; j < col; j++) {
			glm::vec3 rowDiff;
			if (i == 0) {
				rowDiff = terrainVertices[i + 1][j] - terrainVertices[i][j];
			}
			else if (i == row - 1) {
				rowDiff = terrainVertices[i][j] - terrainVertices[i - 1][j];
			}
			else {
				rowDiff = terrainVertices[i + 1][j] - terrainVertices[i - 1][j];
			}
			glm::vec3 colDiff;
			if (j == 0) {
				colDiff = terrainVertices[i][j + 1] - terrainVertices[i][j];
			}
			else if (j == col - 1) {
				colDiff = terrainVertices[i][j] - terrainVertices[i][j - 1];
			}
			else {
				colDiff = terrainVertices[i][j + 1] - terrainVertices[i][j - 1];
			}
			glm::vec3 curNorm = glm::cross(colDiff, rowDiff);
			curNorm = glm::normalize(curNorm);
			curRow.push_back(curNorm);
		}
		terrainNormals.push_back(curRow);
	}
}

void Terrain::perlinNoise() {
	srand(Window::seed);
	perlinNoiseSeed.clear();

	for (int i = 0; i < row; i++) {
		std::vector<float> curRow;
		for (int j = 0; j < col; j++) {
			curRow.push_back(0);
		}
		perlinNoiseSeed.push_back(curRow);
	}

	float scale = 1;
	float scaleAcc = 1;
	for (int o = 0; o < nOctaves; o++) {
		int numSection = pow(2, o);
		int numNoise = numSection + 1;

		// Generate noise
		std::vector<std::vector<float>> curNoiseSeed;
		for (int i = 0; i < numNoise; i++) {
			std::vector<float> curRow;
			for (int j = 0; j < numNoise; j++) {
				curRow.push_back((float)rand() / RAND_MAX * scale);
			}
			curNoiseSeed.push_back(curRow);
		}

		int diff = row / numSection;
		
		// Build up height of this octave
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				float rt = (float)(i % diff) / diff;
				float ct = (float)(j % diff) / diff;
				float q0 = lerp(rt, curNoiseSeed[i/diff][j/diff], curNoiseSeed[i/diff + 1][j/diff]);
				float q1 = lerp(rt, curNoiseSeed[i/diff][j/diff + 1], curNoiseSeed[i/diff + 1][j/diff + 1]);
				float height = lerp(ct, q0, q1);
				perlinNoiseSeed[i][j] += height;
			}
		}
		
		scale /= scaleBias;
		scaleAcc += scale;
	}

	// Scale the height
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			perlinNoiseSeed[i][j] = perlinNoiseSeed[i][j] / scaleAcc * heightScale - heightScale/2;
		}
	}
}

float Terrain::lerp(float t, float a, float b) {
	float t2 = (1 - cos(t*3.141592653589793)) / 2;
	return (a + t2 * (b - a));
}

void Terrain::setStagePlane() {
	int centerR = row / 2;
	int centerC = col / 2;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			float dist = sqrt(pow(i - centerR, 2) + pow(j - centerC, 2));
			if (dist <= stageR) {
				perlinNoiseSeed[i][j] = 0;
			}
			else if (dist <= smoothR + stageR) {
				float t = (float)(dist - stageR) / (float)smoothR;
				perlinNoiseSeed[i][j] = lerp(t, 0, perlinNoiseSeed[i][j]);
			}
		}
	}
}

void Terrain::generateObjectPosition(Node* object, int amount) {
	int count = 0;
	while (count < amount) {
		int i = (float)rand() / RAND_MAX * row;
		int j = (float)rand() / RAND_MAX * col;
		float dist = sqrt(pow(i - row/2, 2) + pow(j - col/2, 2));
		if (dist <= stageR + 2) continue;
		std::pair<int, int> curPos = std::make_pair(i, j);
		if (usedPositions.insert(curPos).second) {
			Transform* offset = new Transform(glm::translate(glm::mat4(1.0f), terrainVertices[i][j]));
			offset->addChild(object);
			childrenList.push_back(offset);
			count++;
		}
	}
}

void Terrain::switchSpreading() {
	spread = !spread;
}

glm::vec3 Terrain::getPosition(glm::vec2 coord) {
	float tx = fmod(coord.x, 1);
	float ty = fmod(coord.y, 1);
	glm::vec3 p0 = terrainVertices[coord.x / 1][coord.y / 1];
	glm::vec3 p1 = terrainVertices[coord.x / 1][coord.y / 1 + 1];
	glm::vec3 p2 = terrainVertices[coord.x / 1 + 1][coord.y / 1];
	glm::vec3 p3 = terrainVertices[coord.x / 1 + 1][coord.y / 1 + 1];
	glm::vec3 q0 = p0 + ty * (p1 - p0);
	glm::vec3 q1 = p2 + ty * (p3 - p2);
	glm::vec3 r = q0 + tx * (q1 - q0);
	return r;
}