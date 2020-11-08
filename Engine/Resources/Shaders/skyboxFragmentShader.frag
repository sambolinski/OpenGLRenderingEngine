#version 400 core
out vec4 FragColor;
in vec3 Normal;  
in vec4 FragPos;  
in vec2 TextureCoords;
in vec3 worldPosition;

uniform samplerCube skybox;


void main(){
    FragColor = texture(skybox, worldPosition);
}