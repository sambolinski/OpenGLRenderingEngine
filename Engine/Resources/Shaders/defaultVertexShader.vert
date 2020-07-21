#version 330 core
// Structure for matrices
uniform struct Matrices
{
	mat4 projectionMatrix;
	mat4 modelViewMatrix; 
} matrices;


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 outColour;

void main()
{
    gl_Position = matrices.projectionMatrix * matrices.modelViewMatrix * vec4(aPos, 1.0f);
    outColour = aColour;
}