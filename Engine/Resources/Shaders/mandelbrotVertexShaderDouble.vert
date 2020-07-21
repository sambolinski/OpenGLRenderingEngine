#version 330 core
uniform struct Matrices
{
	mat4 projectionMatrix;
	mat4 modelViewMatrix; 
} matrices;

layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = matrices.projectionMatrix * matrices.modelViewMatrix * vec4(aPos, 1.0f);
}