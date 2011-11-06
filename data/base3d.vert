#version 120
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec3 color;

varying vec2 fragtexcoord;
varying vec3 fragcolor;

void main()
{
	fragcolor = color;
	fragtexcoord = texcoord;

	//vec4 v = vec4(position, 1.0f);
	//v = projection * view * model * v;

	//gl_Position = v;
	gl_Position = projection * view * model * position;
}
