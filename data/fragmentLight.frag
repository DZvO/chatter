varying vec3 fragPosition;
varying vec4 fragColor;
varying vec3 fragNormal;
varying vec2 fragTexCoord;
varying vec3 fragLightPos;
varying vec3 fragLightCol;

uniform sampler2D texture;

void main ()
{
	//Will be used for attenuation.
	float dist = length(fragLightPos - fragPosition);

	//Get a lighting direction vector from the light to the vertex.
	vec3 lightVector = normalize(fragLightPos - fragPosition);

	//Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
	//pointing in the same direction then it will get max illumination.
	float diffuse = max(dot(fragNormal, lightVector), 0.1);

	//Add attenuation
	diffuse = diffuse * (1.0 / (1.0 + (0.015 * dist * dist)));
	//diffuse += 0.2; //add ambient
	vec4 diff = vec4(diffuse); diff.w = 1.0;
	diff *= vec4(fragLightCol, 1.0);
	diff += 0.01;

	vec4 textureColor = texture2D(texture, fragTexCoord);

	gl_FragColor = textureColor * fragColor * diff;
	//gl_FragColor = textureColor * fragColor;
}
