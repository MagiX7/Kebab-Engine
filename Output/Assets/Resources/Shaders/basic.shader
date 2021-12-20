#type vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vPosition;
out vec3 vNormal;

void main()
{
	vPosition = position;

	gl_Position = projection * view * model * vec4(position, 1);
}

// ========================================================================================

#type fragment
#version 410 core

in vec3 vPosition;
in vec3 vNormal;

void main()
{
	gl_FragColor = vec4(0.5, 0.5, 0.8, 1.0);
}
