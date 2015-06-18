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
	vec3 Light  = normalize(gl_LightSource[0].position.xyz - v);

	// Compute halfway vector
	vec3 Half = normalize(Light + Viewer);

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


	// add factors from pointlight
	vec3 PLight = normalize(gl_LightSource[1].position.xyz - v);
	vec3 PHalf = normalize(Viewer + PLight);

	// attenuate depending on the distance...
	float distance = length(gl_LightSource[1].position.xyz - v);
	
	// get attenuation
	float atten = 1.0 /(gl_LightSource[1].constantAttenuation +   
						gl_LightSource[1].linearAttenuation * distance + 
						gl_LightSource[1].quadraticAttenuation * distance * distance);


	float PB = 1.0;
	if(dot(Normal,PLight)<0.0) PB = 0.0;
	
	// compute geometric terms of diffuse and specular
	float PdiffuseShade = PB * max(dot(Normal, PLight), 0.0);
	float PspecularShade = PB * pow(max(dot(PHalf, Normal), 0.0), gl_FrontMaterial.shininess);
	
	diffuse += atten * PdiffuseShade * gl_FrontLightProduct[1].diffuse;
	specular += atten * PspecularShade * gl_FrontLightProduct[1].specular;
	ambient += atten * gl_FrontLightProduct[1].ambient;

	// add factors from spotlight
	float nDotVP; // normal . light direction
	float nDotHV; // normal . light half vector
	float pf; // power factor
	float spotDot; // cosine of angle between spotlight
	float spotAttenuation; // spotlight attenuation factor 
	
	
	vec3 SLight = normalize(gl_LightSource[2].position.xyz - v);
	vec3 SHalf = normalize(SLight + Viewer);
	distance = length(gl_LightSource[2].position.xyz - v);
	spotDot = dot(normalize(SLight), normalize(gl_LightSource[2].spotDirection));
	if (spotDot < gl_LightSource[2].spotCosCutoff)
		spotAttenuation = 0.0; // light adds no contribution
	else
		spotAttenuation = pow(spotDot, gl_LightSource[2].spotExponent);

	atten = spotAttenuation / (gl_LightSource[2].constantAttenuation +
					gl_LightSource[2].linearAttenuation * distance +
					gl_LightSource[2].quadraticAttenuation * distance * distance);

	// Combine the spotlight and distance attenuation.
	atten *= spotAttenuation;
	nDotVP = max(0.0, dot(Normal, SLight));
	nDotHV = max(0.0, dot(Normal, SHalf)); 
	
	if (nDotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(nDotHV, gl_FrontMaterial.shininess);
	
	ambient += gl_LightSource[2].ambient * atten;
	diffuse += gl_LightSource[2].diffuse * nDotVP * atten;
	specular += gl_LightSource[2].specular * pf * atten; 
	
	// Assign final color
	gl_FragColor = gl_FrontMaterial.emission + ambient + diffuse + specular;
	// Scale with brightness value
	gl_FragColor = gl_FragColor * brightness;
}