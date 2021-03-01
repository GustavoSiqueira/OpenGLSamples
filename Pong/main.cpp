#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"

#define numVAOs 1
#define numVBOs 2

// Rendering program pointer
GLuint program;

// Vertex data buffer pointers
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Projection matrix (orthogonal), view, model and view-model
GLuint projectionLoc;
GLuint mvLoc;
glm::mat4 projectionMatrix, viewMatrix, modelMatrix, viewModelMatrix;

// Camera and object positions
glm::vec3 cameraLocation = glm::vec3(0.0f, 0.0f, 8.0f);
const glm::vec3 paddleStartPos = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 paddleLocation;

// Screen size and aspect ratio
int width, height;
float aspectRatio;

// Movement constraints
const glm::vec3 paddleSpeed = glm::vec3(0.0f, 2.0f, 0.0f);
const float maxY = 0.7f;
const float minY = -maxY;


void processInput(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && paddleLocation.y <= maxY)
		paddleLocation += paddleSpeed * dt;

	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && paddleLocation.y >= minY)
		paddleLocation -= paddleSpeed * dt;
}

float paddleVertices[12] = { 0.1f, -0.1f, 0.0f,
							-0.1f , -0.1f, 0.0f,
							0.1f, 0.1f, 0.0f,
							-0.1f, 0.1f, 0.0f };

void setupVertices() {
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertices), paddleVertices, GL_STATIC_DRAW);
}

void drawBuffer(int bufferIndex, int numVertices) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[bufferIndex]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
}

void drawPlayerPaddle(GLFWwindow* window, double currentTime) {
	glUseProgram(program);

	//Setting perspective
	projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(program, "projection_matrix");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//Setting view-model
	viewMatrix = glm::translate(glm::mat4(1.0f), -cameraLocation);
	modelMatrix = glm::translate(glm::mat4(1.0f), paddleLocation) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 1.25f, 1.0f));

	viewModelMatrix = viewMatrix * modelMatrix;

	mvLoc = glGetUniformLocation(program, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(viewModelMatrix));

	drawBuffer(0, 4);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPlayerPaddle(window, currentTime);
}

void init(GLFWwindow* window) {
	program = createShaderProgram("shaders/vertShader.glsl", "shaders/fragShader.glsl");
	setupVertices();

	glfwGetFramebufferSize(window, &width, &height);
	aspectRatio = (float)width / (float)height;

	paddleLocation = paddleStartPos;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	aspectRatio = (float)width / (float)height;
}

int main()
{
	if (glfwInit() == GLFW_FALSE)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "PongGL", glfwGetPrimaryMonitor(), NULL);

	glfwMakeContextCurrent(window);

	auto glewInitResult = glewInit();

	if (glewInitResult != GLEW_OK)
		return -1;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);

	init(window);

	float dt = 0.0f, 
		time = 0.0f, 
		lastTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		time = (float)glfwGetTime();
		dt = time - lastTime;

		processInput(window, dt);
		display(window, dt);
		glfwPollEvents();
		glfwSwapBuffers(window);

		lastTime = time;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}