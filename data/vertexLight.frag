#version 120
varying vec2 fragtexcoord;
uniform sampler2D texture;

varying vec4 fragcolor;

void main()
{
	gl_FragColor = fragcolor;
	//gl_FragColor = vec4(1, 1, 1, 1);
}
