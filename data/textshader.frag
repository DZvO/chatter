#version 120
varying vec2 tex;
varying vec3 col;
uniform sampler2D texture;

void main()
{
	//gl_FragColor = vec4(1.f, 0.f, 1.f, 1.f);
	//gl_FragColor = texture2D(texture, tex);
	vec4 textureColor = texture2D(texture, tex);
	vec4 tint = vec4(col, 1.0);
	gl_FragColor = textureColor * tint;
}
