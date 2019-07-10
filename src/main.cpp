#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <random>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Box2D/Box2D.h>

#include "render.h"

static void glfwErrorCallback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

SquareRender render;

static void glfwResizeCallback(GLFWwindow *w, int width, int height) {
	glViewport(0, 0, width, height);
	render.changeDimensions(width, height);
}

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //if (key == GLFW_KEY_E && action == GLFW_PRESS) {
}

class Square {
	public:

	float x;
	float y;
	float r;
	b2Body *body;

	Square(float x, float y, float r) {
		this->x = x;
		this->y = y;
		this->r = r;
	}
};

std::vector<Square> squares;

b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);

float timeStep = 1.0f / 60.0f;

void addSquare(float x, float y, float r) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.angle = r;
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.9f;

	body->CreateFixture(&fixtureDef);

	Square s(x, y, r);
	
	s.body = body;

	squares.push_back(s);
}

void drawSquares() {
	for (std::vector<Square>::iterator it = squares.begin(); it != squares.end(); ++it) {
		render.draw(it->x, it->y, 1.0f, 1.0f, it->r);		
	}
}

void updateSquares() {
	world.Step(timeStep, 8, 3);
	for (std::vector<Square>::iterator it = squares.begin(); it != squares.end(); ++it) {
		b2Vec2 p = it->body->GetPosition();
		float x = p.x;
		float y = p.y;
		float r = it->body->GetAngle();
		it->x = x;
		it->y = y;
		it->r = r;
	}
}

int main() {

	b2PolygonShape wall;
	wall.SetAsBox(0.0f, 50.0f);

	b2PolygonShape floor;
	floor.SetAsBox(50.0f, 0.0f);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -25.0f);
	b2Body *groundBody = world.CreateBody(&groundBodyDef);
	groundBody->CreateFixture(&floor, 0.0f);

	b2BodyDef leftWallBodyDef;
	leftWallBodyDef.position.Set(-25.0f, 0.0f);
	b2Body *leftWallBody = world.CreateBody(&leftWallBodyDef);
	leftWallBody->CreateFixture(&wall, 0.0f);

	b2BodyDef rightWallBodyDef;
	rightWallBodyDef.position.Set(25.0f, 0.0f);
	b2Body *rightWallBody = world.CreateBody(&rightWallBodyDef);
	rightWallBody->CreateFixture(&wall, 0.0f);

	glfwSetErrorCallback(glfwErrorCallback);

	if (!glfwInit()) {
		std::cout << "Bad Things with GLFW" << std::endl;	
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "My Title", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, glfwResizeCallback);  
	glfwSetKeyCallback(window, glfwKeyCallback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);

	render.setup();

	addSquare(0.0f, 3.0f, 0.0f);
	addSquare(1.0f, 6.0f, 0.0f);
	addSquare(2.0f, 9.0f, 0.0f);
	addSquare(3.0f, 3.0f, 0.0f);
	addSquare(4.0f, 6.0f, 0.0f);
	addSquare(5.0f, 9.0f, 0.0f);
	addSquare(6.0f, 3.0f, 0.0f);
	addSquare(7.0f, 6.0f, 0.0f);
	addSquare(8.0f, 9.0f, 0.0f);

	auto previous = std::chrono::high_resolution_clock::now();
	double lag = 0.0;
	while (!glfwWindowShouldClose(window)) {
		auto current = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - previous);
		double elapsed = duration.count();
		previous = current;
		lag += elapsed;

		glfwPollEvents();

		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while (lag >= 20) {
			updateSquares();
			lag -= 20;
		}

		drawSquares();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
