#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];

string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";

	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char * log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}

	return foundError;
}

GLuint createShaderProgram() {
	
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;
	

	string vertShaderStr = readShaderSource("vertShader.glsl");
	string fragShaderSrc = readShaderSource("fragShader.glsl");

	const char* vshaderSource = vertShaderStr.c_str();

	const char* fshaderSource = fragShaderSrc.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	
	glCompileShader(vShader);

	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		printShaderLog(vShader);
	}

	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		printShaderLog(fShader);
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	checkOpenGLError();
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog(program);
	}

	return program;
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

float x = 0.0f;
float y = 0.0f;
float z = -1;

float inc_x = 0.01f;
float inc_y = 0.005f;

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	x += inc_x;
	y += inc_y;
	z -1.0f;

	if (abs(x) >= 0.5) 
		inc_x *= -1.0f;

	if (abs(y) >= 0.5)
		inc_y *= -1.0f;

	GLuint xOffsetLoc = glGetUniformLocation(renderingProgram, "xOffset");
	GLuint yOffsetLoc = glGetUniformLocation(renderingProgram, "yOffset");
	GLuint zOffsetLoc = glGetUniformLocation(renderingProgram, "zOffset");

	glProgramUniform1f(renderingProgram, xOffsetLoc, x);
	glProgramUniform1f(renderingProgram, yOffsetLoc, y);
	glProgramUniform1f(renderingProgram, zOffsetLoc, z);

	glPointSize(1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "Very Simple App", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}