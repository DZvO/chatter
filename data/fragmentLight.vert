uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 color;

varying vec3 fragPosition;
varying vec4 fragColor;
varying vec3 fragNormal;
varying vec2 fragTexCoord;

void main ()
{
	fragTexCoord = texcoord;

	//transform the vertex into eye space.
	fragPosition = vec3(view * model * position);

	fragColor = color;

	//transform the normal's orientation into eye space.
	fragNormal = vec3(view * model * vec4(normal, 0.0));

	gl_Position = projection * view * model * position;
}
