#version 120
//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;
uniform mat4 mvp;

attribute vec3 position;
attribute vec2 texcoord;
attribute vec4 color;

varying vec2 fragTexcoord;
varying vec4 fragColor;

void main()
{
	fragTexcoord = texcoord;
	fragColor = color;
	//gl_Position = projection * view * model * vec4(position.x, position.y, 0.0f, 1.0f);
	gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0f);
}
