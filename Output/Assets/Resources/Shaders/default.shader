#type vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform float textureAlpha;
uniform vec3 ambientColor;
uniform vec3 camPos;

out vec3 vPosition;
out vec3 vAmbientColor;
out vec2 vTexCoords;
out vec3 vCamPos;
out vec3 vNormal;
out float vTextureAlpha;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);

	vTexCoords = texCoords;
	vPosition = vec3(model * vec4(position, 1));
	//vNormal = normalMatrix * normal;
	vNormal = normalize((model * vec4(normal, 0.0)).xyz);
	vAmbientColor = ambientColor;
	vTextureAlpha = textureAlpha;

	vCamPos = camPos;
}


#type fragment
#version 430 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 camPos;

in vec3 vAmbientColor;
in float vTextureAlpha;

out vec4 fragColor;

uniform sampler2D tex;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);
	
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// Specular shading
	//vec3 reflectDir = reflect(-lightDir, normal);
	vec3 reflectDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	// Combine results -> ONLY WITH TEXTURES !!!
	//vec3 ambient = light.ambient * vec3(texture(material.diffuse.xyz, vTexCoords));
	//vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse.xyz, vTexCoords));
	//vec3 specular = light.specular * spec * vec3(texture(material.specular.xyz, vTexCoords));

	vec3 ambient = light.ambient * material.diffuse * vAmbientColor;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	return (ambient + diffuse + specular);
}


void main()
{
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(camPos - vPosition);
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	fragColor = texture(tex , vTexCoords) * vTextureAlpha * vec4(result, 1);
}

