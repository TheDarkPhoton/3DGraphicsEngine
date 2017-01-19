/*
 * dpEntity.h
 *
 *  Created on: 7 Aug 2014
 *      Author: darkphoton
 */

#ifndef DPENTITY_H_
#define DPENTITY_H_

#include "dpBlockAsset.h"
#include "dpOrientation.h"
#include "dpMaterial.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

using namespace glm;
using namespace std;

class dpEntity {

protected:
	dpOrientation _orientation;
	mat4 _matrix;

public:
	dpEntity();
	virtual ~dpEntity();
};

class dpBlock: public dpEntity{
	dpBlockAsset& _asset;
	GLuint _asset_id;

public:
	dpBlock(dpBlockAsset& asset, GLuint id);

	mat4& GetMatrix();
	dpMaterial& GetMaterial();

	dpModelResources GetObjects();
	GLuint GetTexture();

	dpOrientation* GetOrientationPtr();
};

class dpCamera{
	dpOrientation* _reference;
	dpOrientation _offset;
	float _rotX;
	float _rotY;

public:
	dpCamera();
	~dpCamera();

	mat4 GetMatrix();
	GLvoid SetReference(dpOrientation* ref);

	GLvoid RotateX(GLfloat angle);
	GLvoid RotateY(GLfloat angle);
	GLvoid RotateZ(GLfloat angle);
	GLvoid TranslateZ(GLfloat amount);
};

class dpPlayer{
	dpOrientation* _reference;

public:
	dpPlayer();

	mat4& GetMatrix();
	GLvoid SetReference(dpOrientation* ref);

	GLvoid RotateX(GLfloat angle);
	GLvoid RotateY(GLfloat angle);
	GLvoid RotateZ(GLfloat angle);
	GLvoid TranslateX(GLfloat amount);
	GLvoid TranslateY(GLfloat amount);
	GLvoid TranslateZ(GLfloat amount);
};
#endif /* DPENTITY_H_ */
