#version 400 core
// Structure for matrices
uniform struct Matrices
{
	mat4 projectionMatrix;
	mat4 modelViewMatrix;
	mat3 normalMatrix;  
} matrices;


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;



out vec2 outTexture;
out vec3 outNormal;
out vec4 outPosition;

void main()
{
    gl_Position = matrices.projectionMatrix * matrices.modelViewMatrix * vec4(aPos, 1.0f);
    
    outTexture = aTexture;
    outNormal = normalize(matrices.normalMatrix * aNormal);
    outPosition = matrices.modelViewMatrix * vec4(aPos, 1.0f);
}