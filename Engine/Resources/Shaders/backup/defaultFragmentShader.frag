#version 400 core

out vec4 FragColor;

in vec2 outTexture;
in vec3 outNormal;
in vec4 outPosition;

uniform sampler2D ourTexture;
uniform bool enableTexture;

struct Light{
    vec4 position;
    vec3 ambience;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
    float exponent;
    float cutoff;
};
struct Material{
    vec3 ambience;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
//Lighting
uniform Light[1] lights;
uniform Material material1;

// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
// Adapted to BlinnPhong using lab 6
vec3 BlinnPhongModel(Light light, vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 n = eyeNorm;
	vec3 ambient = light.ambience * material1.ambience;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light.diffuse * material1.diffuse * sDotN;
	vec3 specular = vec3(0.0f);
	vec3 h = normalize(v + s);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light.specular * material1.specular * pow(max(dot(h, n), 0.0f), material1.shininess + eps);
	

	return ambient + diffuse + specular;

}
void main() 
{    
    
    vec4 textureColour = texture(ourTexture, outTexture);
    vec3 lighting = BlinnPhongModel(lights[0], outPosition, normalize(outNormal));
    if(enableTexture){
        FragColor = textureColour*vec4(lighting, 1.0f);
    }else{
        FragColor = vec4(lighting, 1.0f);
    }
}