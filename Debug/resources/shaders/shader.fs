#version 150 core

uniform vec4 frag_colour;
uniform sampler2D frag_texture;
uniform mat4 frag_world_transform;
uniform mat4 frag_camera_transform;

uniform struct LightParameters {
	vec3 position;					//Light source
	vec3 colour;					//Colour of the light
	float attenuation;				//The speed of decrease in brightness
	float ambient;					//Ambient percentage
} frag_light_params;

uniform struct SpecularParameters {
	float shininess;				//Shininess of the material
	vec3 colour;					//Colour of reflection
} frag_reflection_params;

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coordinates;

out vec4 final_colour;

void main(){
	vec4 surfaceColour = frag_colour * texture(frag_texture, frag_texture_coordinates);
	vec3 frag_normal = normalize(transpose(inverse(mat3(frag_world_transform))) * frag_normal);
	vec3 surfacePosition = vec3(frag_world_transform * vec4(frag_position, 1));
	vec3 directionToLight = normalize(frag_light_params.position - surfacePosition);

	//ambient model
	vec3 AMBIENT = frag_light_params.ambient * surfaceColour.rgb * frag_light_params.colour;

	//diffuse model
	float diffuseCoefficient = max(0.0, dot(frag_normal, directionToLight));
	
	vec3 DIFFUSE = diffuseCoefficient * surfaceColour.rgb * frag_light_params.colour;
	
	//specular model
	float specularCoefficient = 0.0;
	
	if (diffuseCoefficient > 0){
		//vec3 incidenceVector = -directionToLight;
		//vec3 reflectionVector = reflect(incidenceVector, frag_normal);
		vec3 cameraPosition = vec3(frag_camera_transform * vec4(1,1,1,1));
		vec3 directionToCamera = normalize(cameraPosition - surfacePosition);
		
		vec3 halfAngle = normalize(directionToLight + directionToCamera);
		float blinnTerm = clamp(dot(frag_normal, halfAngle), 0, 1);
		
		//float cosineAngle = max(0.0, dot(directionToCamera, reflectionVector));
		//specularCoefficient = pow(cosineAngle, frag_reflection_params.shininess);
		specularCoefficient = pow(blinnTerm, frag_reflection_params.shininess);
	}
	
	vec3 SPECULAR = specularCoefficient * frag_reflection_params.colour * frag_light_params.colour;
	
	//attenuation model
	float distanceToLight = length(frag_light_params.position - surfacePosition);	
	
	float ATTENUATION = 1.0 / (1.0 + frag_light_params.attenuation * pow(distanceToLight, 2));
	
	//linear colour
	vec3 linearColour = AMBIENT + ATTENUATION * (DIFFUSE + SPECULAR);
	
	//final colour
	vec3 gamma = vec3(1.0/2.2);
    final_colour = vec4(pow(linearColour, gamma), surfaceColour.a);
	//final_colour = vec4(linearColour, 1);
}
