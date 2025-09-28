#include "Terrain.h"

Terrain::Terrain(float size, float y) {
	setupMesh(size, y);
}

Terrain::~Terrain() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Terrain::setupMesh(float size, float y) {
	float half = size * 0.5f;

	float vertices[] = {
		// pos                // normal        // uv
		-half, y, -half,      0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 half, y, -half,      0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 half, y,  half,      0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		-half, y,  half,      0.0f, 1.0f, 0.0f,  0.0f, 1.0f
	};

	unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
	indexCount = 6;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//позиция
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	//нормаль
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	//UV
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}

void Terrain::Draw() const {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
