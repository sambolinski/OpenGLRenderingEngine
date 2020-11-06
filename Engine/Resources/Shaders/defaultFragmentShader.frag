#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec4 FragPos;  
in vec2 TextureCoords;

struct Light{
    vec3 position;
    vec3 ambience;
    vec3 diffuse;
    vec3 specular;
};
struct Material{
    vec3 ambience;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light[1] lights; 
uniform Material material;
uniform sampler2D ourTexture;
uniform bool applyTexture;
uniform bool renderSkybox;

// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
// Adapted to BlinnPhong using lab 6
vec3 BlinnPhongModel(Light light, vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light.position - eyePosition.xyz));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 n = eyeNorm;
	vec3 ambient = light.ambience * material.ambience;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light.diffuse * material.diffuse * sDotN;
	vec3 specular = vec3(0.0f);
	vec3 h = normalize(v + s);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light.specular * material.specular * pow(max(dot(h, n), 0.0f), material.shininess + eps);
	

	return ambient + diffuse + specular;

}

void main()
{
    vec3 lighting = BlinnPhongModel(lights[0],FragPos, normalize(Normal));
    vec4 textureColour = texture(ourTexture, TextureCoords);
    FragColor = textureColour*vec4(lighting, 1.0);
} 
/*
 if(renderSkybox){
        FragColor = texture(CubeMapTex, worldPosition);
    }else{
        vec3 lighting = BlinnPhongModel(lights[0],FragPos, normalize(Normal));
        vec4 textureColour = texture(ourTexture, TextureCoords);
        FragColor = textureColour*vec4(lighting, 1.0);
    }
*/
/*
    vec3 ambient = lights[0].ambience;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lights[0].position - FragPos.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lights[0].diffuse;
            
    vec3 result = (ambient + diffuse) * material.diffuse;
    vec4 textureColour = vec3(0,0,0,0);
    if(applyTexture)
        textureColour = texture(ourTexture, TextureCoords);
    FragColor = textureColour*vec4(result, 1.0);
*/