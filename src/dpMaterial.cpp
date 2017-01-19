/*
 * dpMaterial.cpp
 *
 *  Created on: 19 Aug 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpMaterial.h"

dpMaterial::dpMaterial(string name, glm::vec4 surfaceCol, GLfloat sharpness, GLfloat shininess, glm::vec3 specularCol)
	: _name(name), _surfaceCol(surfaceCol), _specularCol(specularCol) {
	_sharpness = 40 * glm::clamp(sharpness, 0.0f, 1.0f) + 10;
	_shininess = glm::clamp(shininess, 0.0f, 1.0f);
}

string& dpMaterial::Name(){
	return _name;
}
GLfloat dpMaterial::GetSharpness(){
	return _sharpness;
}
glm::vec3 dpMaterial::GetSpecularCol(){
	return _specularCol * _shininess;
}
glm::vec4 dpMaterial::GetSurfaceCol(){
	return _surfaceCol;
}
