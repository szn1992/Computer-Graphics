// Fragment shader for per-pixel Phong interpolation and shading.

// The "varying" keyword means that the parameter's value is interpolated
// between the nearby vertices.
varying vec3 N;
varying vec3 v;

uniform float brightness;

void main()
{
    // The scene's ambient light.
    vec4 ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	// The normal vectors is generally not normalized after being
	// interpolated across a triangle.  Here we normalize it.
	vec3 Normal = normalize(N);

	// Since the vertex is in eye space, the direction to the
	// viewer is simply the normalized vector from v to the
	// origin.
	vec3 Viewer = -normalize(v);

	// Get the lighting direction and normalize it.
	vec3 Light  = normalize(gl_LightSource[0].position.xyz);

	// Compute halfway vector
	vec3 Half = normalize(Viewer+Light);

	// Compute factor to prevent light leakage from below the
	// surface
	float B = 1.0;
	if(dot(Normal, Light)<0.0) B = 0.0;

	// Compute geometric terms of diffuse and specular
	float diffuseShade = max(dot(Normal, Light), 0.0);
	float specularShade = 
	  B * pow(max(dot(Half, Normal), 0.0), gl_FrontMaterial.shininess);

	// Compute product of geometric terms with material and
	// lighting values
	vec4 diffuse = diffuseShade * gl_FrontLightProduct[0].diffuse;
	vec4 specular = specularShade * gl_FrontLightProduct[0].specular;
	ambient += gl_FrontLightProduct[0].ambient;

	// Assign final color
	gl_FragColor = ambient + diffuse + specular + gl_FrontMaterial.emission;
	// Scale with brightness value
	gl_FragColor = gl_FragColor * brightness;



	// Get the Point light direction and normalize it.
	// -v
	vec3 pLight = normalize(gl_LightSource[1].position.xyz-v);

	// Compute halfway vector
	vec3 pHalf = normalize(Viewer + pLight);

	// Compute factor to prevent light leakage from below the
	// surface
	float pB = 1.0;
	if(dot(Normal, Light)<0.0) pB = 0.0;

	// Compute geometric terms of diffuse and specular
	float pdiffuseShade = max(dot(Normal, pLight), 0.0);
	float pspecularShade = B * pow(max(dot(pHalf, Normal), 0.0), gl_FrontMaterial.shininess);

	// Compute product of geometric terms with material and
	// lighting values
	vec4 pdiffuse = pdiffuseShade * gl_FrontLightProduct[1].diffuse;
	vec4 pspecular = pspecularShade * gl_FrontLightProduct[1].specular;
	vec4 pambient = gl_FrontLightProduct[1].ambient;

	// Compute the distance between pixel and the light source; then compute f.
	float r = length(gl_LightSource[1].position.xyz - v);
	float a = (gl_LightSource[1].constantAttenuation + gl_LightSource[0].constantAttenuation) / 2;
	float b = (gl_LightSource[1].linearAttenuation + gl_LightSource[0].linearAttenuation) / 2;
	float c = (gl_LightSource[1].quadraticAttenuation + gl_LightSource[0].quadraticAttenuation) / 2;
	float f = 1.0 / (a + b * r + c * r * r);

	// Assign final color
	gl_FragColor += (pambient + pdiffuse + pspecular) * f;

	// Set the cartoon effect
	float x,y,z,a2,scale;
	scale = 5.0;
	x = ceil (gl_FragColor.x * scale - 0.3) / scale;
	y = ceil (gl_FragColor.y * scale - 0.6) / scale;
	z = ceil (gl_FragColor.z * scale - 0.3) / scale;
	a2 = gl_FragColor.a;
	gl_FragColor = vec4 (x,y,z,a2);

	// Scale with brightness value
	gl_FragColor = gl_FragColor * brightness;

}