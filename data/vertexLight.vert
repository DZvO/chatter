#version 120
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;       // The position of the light in eye space.

attribute vec4 position;     // Per-vertex position information we will pass in.
attribute vec4 color;        // Per-vertex color information we will pass in.
attribute vec3 normal;       // Per-vertex normal information we will pass in.

varying vec4 fragcolor;          // This will be passed into the fragment shader.

void main()                    // The entry point for our vertex shader.
{
	// Transform the vertex into eye space.
	vec3 modelViewVertex = vec3(view * model * position);
	// Transform the normal's orientation into eye space.
	vec3 modelViewNormal = vec3(view * model * vec4(normal, 0.0));
	// Will be used for attenuation.
	float dist = length(lightPos - modelViewVertex);
	// Get a lighting direction vector from the light to the vertex.
	vec3 lightVector = normalize(lightPos - modelViewVertex);
	// Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
	// pointing in the same direction then it will get max illumination.
	float diffuse = max(dot(modelViewNormal, lightVector), 0.1);
	// Attenuate the light based on distance.
	diffuse = diffuse * (1.0 / (1.0 + (0.25 * dist * dist)));
	// Multiply the color by the illumination level. It will be interpolated across the triangle.
	fragcolor = color * diffuse;// + vec4(0.1, 0.1, 0.1, 1.0);
	// gl_Position is a special variable used to store the final position.
	// Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
	
	gl_Position = projection * view * model * position;
}  
