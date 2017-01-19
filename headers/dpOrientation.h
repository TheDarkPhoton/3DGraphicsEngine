/*
 * dpOrientation.h
 *
 *  Created on: 11 Aug 2014
 *      Author: darkphoton
 */

#ifndef DPORIENTATION_H_
#define DPORIENTATION_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class dpOrientation {
	vec3 _location;
	fquat _direction;
	GLboolean _updated;

	GLvoid OffsetOrientation(const vec3 &axis, float fAngDeg);
public:
	vec3 GetLocation();
	fquat GetDirection();
	GLboolean& Updated();

	GLvoid Rotate(vec3 axis, GLfloat angle);
	GLvoid Translate(vec3 translation);
};

#endif /* DPORIENTATION_H_ */
