#type vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);
}


#type fragment
#version 410 core

layout(location = 0) out vec4 color;

void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
