#ifndef GAME_RENDER
#define GAME_RENDER

#include <glad/glad.h>

void modelMatrix(float *m, float x, float y, float h, float w, float r);
void projectionMatrix(float *m, float h, float w);
void viewMatrix(float *m, float x, float y);

class SquareRender {
	GLuint vbo;
	GLuint vao;
	GLuint shader;

	float width;
	float height;

	float vertices[8] = { -1, -1, 1, -1, 1, 1, -1, 1};

	char const *VertexShader = "#version 330 core\n"
				   "layout(location = 0) in vec2 pos;\n"
				   "\n"
				   "uniform mat3 model;\n"
				   "uniform mat3 view;\n"
				   "uniform mat3 projection;\n"
				   "\n"
				   "void main() {\n"
					   "gl_Position.xyz = projection * view * model * vec3(pos.x, pos.y, 1);\n"
					   "gl_Position.w = 1.0;\n"
				   "}";

	char const *FragmentShader = "#version 330 core\n\
				      out vec3 color;\n\
				      void main() {\n\
					color = vec3(0,0,0);\n\
				      }";
	public:

	void setup(); 
	void draw(float x, float y, float w, float h, float r);
	void changeDimensions(float width, float height);

};
#endif
