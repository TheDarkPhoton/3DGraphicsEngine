/*
 * dpBlock.h
 *
 *  Created on: 7 Aug 2014
 *      Author: darkphoton
 */

#ifndef DPBLOCK_H_
#define DPBLOCK_H_

#include "dpMaterial.h"

#include <string>
#include <vector>

using namespace std;

struct dpModelResources{
public:
	GLuint vertexCount;
	GLuint vertexBufferObject;
	GLuint elementBufferObject;
	GLuint vertexArrayObject;
};

struct dpModelData{
public:
	vector<GLfloat> vertices;
	vector<GLuint> indices;
};

class dpBlockAsset {
	string _name;

	vector<dpModelResources> _models;
	dpMaterial& _material;
	GLuint _texture;

public:
	dpBlockAsset(string name, vector<dpModelResources>& models, GLuint texture, dpMaterial& material);
	GLvoid Delete();

	dpModelResources& GetBlockObjects(GLuint index);
	GLuint& Texture();
	dpMaterial& Material();
	string& Name();

	dpBlockAsset& operator=(dpBlockAsset rhs);
};

#endif /* DPBLOCK_H_ */
