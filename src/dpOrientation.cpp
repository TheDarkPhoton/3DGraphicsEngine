/*
 * dpOrientation.cpp
 *
 *  Created on: 11 Aug 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpOrientation.h"

GLvoid dpOrientation::OffsetOrientation(const vec3 &axis, float fAngDeg){
	float fAngRad = fAngDeg * M_PI / 180;

	vec3 normalised_axis = normalize(axis);

	normalised_axis = normalised_axis * sinf(fAngRad / 2.0f);
	float scalar = cosf(fAngRad / 2.0f);

	fquat offset(scalar, normalised_axis.x, normalised_axis.y, normalised_axis.z);

	if(false)
		_direction = _direction * offset;
	else
		_direction = offset * _direction;

	_direction = normalize(_direction);
}

vec3 dpOrientation::GetLocation(){
	return _location;
}
fquat dpOrientation::GetDirection(){
	return _direction;
}
GLboolean& dpOrientation::Updated(){
	return _updated;
}

GLvoid dpOrientation::Rotate(vec3 axis, GLfloat angle){
	OffsetOrientation(axis, angle);
	_updated = true;
}
GLvoid dpOrientation::Translate(vec3 translation){
	_location += translation;
	_updated = true;
}
