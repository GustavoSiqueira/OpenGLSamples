#version 430

layout (location=0) in vec3 position;

uniform mat4 projection_matrix;
uniform mat4 mv_matrix;

void main() {
	gl_Position = projection_matrix * mv_matrix * vec4(position, 1.0);
}