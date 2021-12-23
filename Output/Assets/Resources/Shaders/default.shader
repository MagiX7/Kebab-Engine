#type vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec3 ambientColor;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vAmbientColor;
out vec2 vTexCoords;

void main()
{
	vTexCoords = texCoords;
	vPosition = vec3(model * vec4(position, 1));
	//vNormal = normalMatrix * normal;
	vNormal = normalize((model * vec4(normal, 0.0)).xyz);
	vAmbientColor = ambientColor;

	gl_Position = projection * model * view * vec4(position, 1);
}



#type fragment
#version 410 core

in vec3 vPosition;
in vec3 vNormal;
in vec3 vAmbientColor;
in vec2 vTexCoords;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 lightPos = vec3(20, 10, 5);
	vec3 lightDir = normalize(lightPos - vPosition);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 lightColor = vec3(1, 1, 0.1);
	vec3 diffuse = diff * lightColor;

	vec3 ambient = vec3(0.8, 0.2, 0.2) ;
	vec3 result = (ambient + diffuse) * vAmbientColor;

	fragColor = texture(tex, vTexCoords) * vec4(result, 1.0);
}