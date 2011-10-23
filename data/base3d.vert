#version 120
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 color;

varying vec2 tex;
varying vec3 col;

void main()
{
	tex = texcoord;
	col = color;
	gl_Position = projection * model * view * vec4(position, 1.0f);
}
