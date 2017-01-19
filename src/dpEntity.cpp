/*
 * dpEntity.cpp
 *
 *  Created on: 7 Aug 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpEntity.h"
#include "dpUtility.h"
#include "dpGameResources.h"

#define _USE_MATH_DEFINES
#include <math.h>

dpEntity::dpEntity(){

}
dpEntity::~dpEntity(){

}

dpBlock::dpBlock(dpBlockAsset& asset, GLuint id): _asset(asset){
	_asset_id = id;
}
mat4& dpBlock::GetMatrix(){
	if (_orientation.Updated()){
		_matrix = mat4();
		_matrix = translate(_matrix, _orientation.GetLocation());
		_matrix *= mat4_cast(_orientation.GetDirection());
		_orientation.Updated() = false;
	}

	return _matrix;
}
dpOrientation* dpBlock::GetOrientationPtr(){
	return &_orientation;
}
dpModelResources dpBlock::GetObjects(){
	return _asset.GetBlockObjects(_asset_id);
}
GLuint dpBlock::GetTexture(){
	return _asset.Texture();
}
dpMaterial& dpBlock::GetMaterial(){
	return _asset.Material();
}

dpCamera::dpCamera(){
	_reference = NULL;
	_rotX = M_PI / 4;
	_rotY = 0;
	_offset.Translate(vec3(0,0,-5));
	_offset.Rotate(vec3(1,0,0), 45);
}
dpCamera::~dpCamera(){
}
mat4 dpCamera::GetMatrix(){
	mat4 matrix;

	matrix = translate(matrix, _offset.GetLocation());
//	matrix *= mat4_cast(_offset.GetDirection());
	matrix = rotate(matrix, _rotX, vec3(1,0,0));
	matrix = rotate(matrix, _rotY, vec3(0,1,0));

	if (_reference){
		matrix *= mat4_cast(inverse(_reference->GetDirection()));
		matrix = translate(matrix, -_reference->GetLocation());
	}

	return matrix;
}
GLvoid dpCamera::SetReference(dpOrientation* ref){
	_reference = ref;
}
GLvoid dpCamera::RotateX(GLfloat angle){
	GLfloat rad = angle * M_PI / 180;
	if (!(_rotX + rad > M_PI / 2 || _rotX + rad < -M_PI / 2))
		_rotX += rad;

	//_offset.Rotate(vec3(1, 0, 0), angle);
}
GLvoid dpCamera::RotateY(GLfloat angle){
	_rotY += angle * M_PI / 180;

	if (_rotY >= 2 * M_PI) _rotY -= 2 * M_PI;
	else if (_rotY < 0) _rotY += 2 * M_PI;

	//_offset.Rotate(vec3(0, 1, 0), angle);
}
GLvoid dpCamera::RotateZ(GLfloat angle){
	_offset.Rotate(vec3(0, 0, 1), angle);
}
GLvoid dpCamera::TranslateZ(GLfloat amount){
	_offset.Translate(vec3(0, 0, amount));
}

dpPlayer::dpPlayer(){
	_reference = NULL;
}
GLvoid dpPlayer::SetReference(dpOrientation* ref){
	_reference = ref;
}
GLvoid dpPlayer::RotateX(GLfloat angle){
	if (_reference){
		vec4 right(1,0,0,0);
		right = right * mat4_cast(inverse(_reference->GetDirection()));
		_reference->Rotate(vec3(right), angle);
	}
}
GLvoid dpPlayer::RotateY(GLfloat angle){
	if (_reference){
		vec4 up(0,1,0,0);
		up = up * mat4_cast(inverse(_reference->GetDirection()));
		_reference->Rotate(vec3(up), angle);
	}
}
GLvoid dpPlayer::RotateZ(GLfloat angle){
	if (_reference){
		vec4 forward(0,0,1,0);
		forward = forward * mat4_cast(inverse(_reference->GetDirection()));
		_reference->Rotate(vec3(forward), angle);
	}
}
GLvoid dpPlayer::TranslateX(GLfloat amount){
	if (_reference){
		vec4 right(1,0,0,0);
		right = right * mat4_cast(inverse(_reference->GetDirection()));
		_reference->Translate(amount * vec3(right));
	}
}
GLvoid dpPlayer::TranslateY(GLfloat amount){
	if (_reference){
		vec4 up(0,1,0,0);
		up = up * mat4_cast(inverse(_reference->GetDirection()));
		_reference->Translate(amount * vec3(up));
	}
}
GLvoid dpPlayer::TranslateZ(GLfloat amount){
	if (_reference){
		vec4 forward(0,0,1,0);
		forward = forward * mat4_cast(inverse(_reference->GetDirection()));
		_reference->Translate(amount * vec3(forward));
	}
}
