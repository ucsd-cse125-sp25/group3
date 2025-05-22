#version 330 core

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 fragNormal;
in vec3 fragPos;
in vec2 uv;

// uniforms used for lighting
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side

uniform vec3 LightDirection = normalize(vec3(1, 5, 2));
uniform vec3 LightColor = vec3(1.0f);
uniform float AmbientStrength = 0.1f;
uniform float SpecularStrength = 0.5f;

uniform sampler2D tex;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{	vec3 ambient = AmbientStrength * LightColor;

	vec3 norm = normalize(fragNormal);
	
	vec3 diffuse = LightColor * max(0.0f, dot(LightDirection, norm));

	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = ambient + diffuse;

	// Diffuse reflectance
	//vec3 reflectance = irradiance * texture(tex, uv).rgb;
	vec3 reflectance = irradiance * DiffuseColor;

	// Gamma correction
	fragColor = vec4(sqrt(reflectance), 1);
	// fragColor = vec4(fragNormal, 1);

	vec3 temp = fragPos; 
}