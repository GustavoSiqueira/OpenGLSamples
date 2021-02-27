mat4 buildTranslate(float x, float y, float z)
{
	mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
					  0.0, 1.0, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  x, y, z, 1.0);

	return trans;
}

mat4 buildRotateX(float rad)
{
	mat4 rot = mat4(1.0, 0.0, 0.0, 0.0,
					0.0, cos(rad), -sin(rad), 0.0,
					0.0, sin(rad), cos(rad), 0.0,
					0.0, 0.0, 0.0, 1.0);

	return rot;
}

mat4 buildRotateY(float rad)
{
	mat4 rot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					-sin(rad), 0.0, cos(rad), 0.0,
					0.0, 0.0, 0.0, 1.0);

	return rot;
}

mat4 buildRotateZ(float rad)
{
	mat4 rot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
					sin(rad), cos(rad), 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0);

	return rot;
}

mat4 buildScale(float x, float y, float z)
{
	mat4 scale = mat4(x, 0.0, 0.0, 0.0,
					  0.0, y, 0.0, 0.0,
					  0.0, 0.0, z, 0,
					  0.0, 0.0, 0.0, 1);

	return scale;
}