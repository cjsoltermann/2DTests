#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "render.h"

void modelMatrix(float *m, float x, float y, float h, float w, float r) {
	m[0] = w * cos(r);
	m[1] = -h * sin(r);
	m[2] = x;
	
	m[3] = w * sin(r);
	m[4] = h * cos(r);
	m[5] = y;

	m[6] = 0;
	m[7] = 0;
	m[8] = 1;
}

void projectionMatrix(float *m, float h, float w) {
	m[0] = 1 / w;
	m[1] = 0;
	m[2] = 0;

	m[3] = 0;
	m[4] = 1 / h;
	m[5] = 0;

	m[6] = 0;
	m[7] = 0;
	m[8] = 1;
}

void viewMatrix(float *m, float x, float y) {
	m[0] = 1;
	m[1] = 0;
	m[2] = -x;
	m[3] = 0;
	m[4] = 1;
	m[5] = -y;
	m[6] = 0;
	m[7] = 0;
	m[8] = 1;
}

void SquareRender::setup() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	char log[2048];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShader, NULL);
	glCompileShader(vertexShader);

	glGetShaderInfoLog(vertexShader, 2048, NULL, log);
	printf("Vertex: %s\n", log);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShader, NULL);
	glCompileShader(fragmentShader);

	glGetShaderInfoLog(fragmentShader, 2048, NULL, log);
	printf("Fragment: %s\n", log);

	shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);

	glDetachShader(shader, vertexShader);
	glDetachShader(shader, fragmentShader);
}

void SquareRender::draw(float x, float y, float w, float h, float r) {
	glUseProgram(shader);
	float model[9];
	modelMatrix(model, x, y, w, h, r);
	float view[9];
	viewMatrix(view, 0, 0);
	float projection[9];
	projectionMatrix(projection, this->height, this->width);

	GLuint modelID = glGetUniformLocation(shader, "model");
	GLuint viewID = glGetUniformLocation(shader, "view");
	GLuint projectionID = glGetUniformLocation(shader, "projection");

	glUniformMatrix3fv(modelID, 1, GL_TRUE, model);
	glUniformMatrix3fv(viewID, 1, GL_FALSE, view);
	glUniformMatrix3fv(projectionID, 1, GL_FALSE, projection);

	glLineWidth(5);
	glEnableVertexAttribArray(0);
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDisableVertexAttribArray(0);
}

void SquareRender::changeDimensions(float width, float height) {
	float ratio = width / height;
	this->height = 25.0f;
	this->width = this->height * ratio;
}
