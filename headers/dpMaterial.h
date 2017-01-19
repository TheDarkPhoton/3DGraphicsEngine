/*
 * dpMaterial.h
 *
 *  Created on: 19 Aug 2014
 *      Author: darkphoton
 */

#ifndef DPMATERIAL_H_
#define DPMATERIAL_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <string>

using namespace std;

class dpMaterial {
	string _name;
	glm::vec4 _surfaceCol;
	GLfloat _sharpness;
	GLfloat _shininess;
	glm::vec3 _specularCol;

public:
	dpMaterial(string name, glm::vec4 surfaceCol, GLfloat sharpness, GLfloat shininess, glm::vec3 specularCol);

	string& Name();
	GLfloat GetSharpness();
	glm::vec3 GetSpecularCol();
	glm::vec4 GetSurfaceCol();
};

#endif /* DPMATERIAL_H_ */
