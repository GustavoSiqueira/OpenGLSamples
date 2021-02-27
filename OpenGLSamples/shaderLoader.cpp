#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

using namespace std;

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

GLuint createShaderProgram() {

	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	const char* vshaderSource = 
		"#version 430\n"
		"void main(void)\n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

	const char* fshaderSource =
		"#version 430\n"
		"out vec4 color;\n"
		"void main(void)\n"
		"{ if (gl_FragCoord.x < 200) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(0.0, 0.0, 1.0, 1.0); }";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	glCompileShader(vShader);

	//checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		//printShaderLog(vShader);
	}

	glCompileShader(fShader);
	//checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		//printShaderLog(fShader);
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	//checkOpenGLError();
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		//printProgramLog(program);
	}

	return program;
}

GLuint createVertexShader(const char* fromFile) {

}

GLuint createFragmentShader(const char* fromFile) {

}