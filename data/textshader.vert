#version 120

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

attribute vec3 position;
attribute vec2 texcoord;

varying vec2 tex;

void main()
{
	tex = texcoord;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}
