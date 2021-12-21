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

out vec3 vColor;

void main()
{
	vColor = ambientColor;

	gl_Position = projection * model * view * vec4(position, 1);
}



#type fragment
#version 410 core

in vec3 vColor;

void main()
{
	gl_FragColor = vec4(vColor, 0.5);
}