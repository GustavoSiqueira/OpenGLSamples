#version 430

uniform float xOffset;
uniform float yOffset;
uniform float zOffset;

void main(void)
{
	if (gl_VertexID == 0) gl_Position = vec4(-0.5 + xOffset, -0.5 + yOffset, zOffset, 1.0);
	else if (gl_VertexID == 1) gl_Position = vec4(0 + xOffset, 0.5 + yOffset, zOffset, 1.0);
	else gl_Position = vec4(0.5 + xOffset, -0.5 + yOffset, zOffset, 1.0);
}