#version 150 core

uniform mat4 vert_projection;
uniform mat4 vert_camera;
uniform mat4 vert_world;

in vec3 vert_position;
in vec3 vert_normal;
in vec2 vert_texture_coordinates;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coordinates;

void main(){
	frag_position = vert_position;
	frag_normal = vert_normal;
	frag_texture_coordinates = vert_texture_coordinates;
	
	gl_Position = vert_projection * vert_camera * vert_world * vec4(vert_position, 1);
}
