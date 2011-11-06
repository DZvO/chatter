#version 120
uniform sampler2D texture;

varying vec2 fragtexcoord;
varying vec3 fragcolor;

void main()
{
	//vec4 textureColor = texture2D(texture, fragtexcoord);
	//vec4 tint = vec4(fragcolor, 1.0);
	//gl_FragColor = textureColor * tint;

	gl_FragColor = vec4(1.0);

	//float odd = floor(mod(gl_FragCoord.y, 2.0));
	//odd += floor(mod(gl_FragCoord.x, 2.0));
	//gl_FragColor = vec4(fragcolor.xyz, odd);
}
