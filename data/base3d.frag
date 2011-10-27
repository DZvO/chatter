#version 120
varying vec2 fragtexcoord;
varying vec3 fragcolor;
uniform sampler2D texture;

void main()
{
	//gl_FragColor = vec4(1.f, 0.f, 1.f, 1.f);

	//gl_FragColor = texture2D(texture, tex);

	vec4 textureColor = texture2D(texture, fragtexcoord);
	//vec4 textureColor = vec4(1.0);
	vec4 tint = vec4(fragcolor, 1.0);
	gl_FragColor = textureColor * tint;
	//gl_FragColor = vec4(1.0);
}
