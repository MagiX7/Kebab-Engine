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

out vec3 vPosition;
out vec3 vAmbientColor;
out vec2 vTexCoords;
out float vTextureAlpha;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);

	vTexCoords = texCoords;
	vPosition = vec3(model * vec4(position, 1));
	//vNormal = normalMatrix * normal;
	//vNormal = normalize((model * vec4(normal, 0.0)).xyz);
	vAmbientColor = ambientColor;
	vTextureAlpha = textureAlpha;
}


#type fragment
#version 430 core

in vec3 vPosition;
in vec3 vAmbientColor;
in vec2 vTexCoords;
in float vTextureAlpha;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, vTexCoords) * vec4(vAmbientColor, vTextureAlpha);
}