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



out vec2 TextureCoords;
out vec3 Normal;
out vec4 FragPos;
out vec3 worldPosition;

void main()
{
    Normal = aNormal;
    gl_Position = matrices.projectionMatrix * matrices.modelViewMatrix * vec4(aPos, 1.0f);
    TextureCoords = aTexture;
    worldPosition = aPos;
    FragPos = matrices.modelViewMatrix * vec4(aPos, 1.0f);
    
}