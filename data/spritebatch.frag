#version 120
varying vec2 fragTexcoord;
varying vec4 fragColor;
uniform sampler2D texture;

void main()
{
	//gl_FragColor = vec4(1.f, 0.f, 1.f, 1.f);
	//gl_FragColor = texture2D(texture, tex);

	vec4 textureColor = texture2D(texture, fragTexcoord);
	if(textureColor.a < 0.1)
		discard;
	vec4 tint = fragColor;
	gl_FragColor = textureColor * tint;
}
