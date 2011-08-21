#version 120
varying vec2 tex;
uniform sampler2D texture;

void main()
{
	//gl_FragColor = vec4(1.f, 0.f, 1.f, 1.f);
	gl_FragColor = texture2D(texture, tex);
}
