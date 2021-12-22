#type vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 ambientColor;

out vec2 TexCoord;
out vec3 ourColor;

void main()
{
    ourColor = ambientColor;
    gl_Position = projection * model * view * vec4(position, 1.0);
    TexCoord = texCoords;
}


#type fragment
#version 410 core

out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, TexCoord) * vec4(ourColor, 1.0);
}