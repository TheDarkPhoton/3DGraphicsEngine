/*
 * dpBlock.cpp
 *
 *  Created on: 7 Aug 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpBlockAsset.h"
#include "dpUtility.h"

dpBlockAsset::dpBlockAsset(string name, vector<dpModelResources>& models, GLuint texture, dpMaterial& material)
	: _name(name), _models(models), _material(material), _texture(texture){

}
GLvoid dpBlockAsset::Delete() {
	for (GLuint i = 0; i < _models.size(); ++i) {
		glDeleteBuffers(1, &_models[i].vertexBufferObject);
		glDeleteBuffers(1, &_models[i].elementBufferObject);
		glDeleteVertexArrays(1, &_models[i].vertexArrayObject);
	}

	glDeleteTextures(1, &_texture);
}

dpModelResources& dpBlockAsset::GetBlockObjects(GLuint index){
	return _models[index];
}
GLuint& dpBlockAsset::Texture(){
	return _texture;
}
dpMaterial& dpBlockAsset::Material(){
	return _material;
}
string& dpBlockAsset::Name(){
	return _name;
}

dpBlockAsset& dpBlockAsset::operator=(dpBlockAsset rhs){
	_name = rhs._name;
	_models = rhs._models;
	_texture = rhs._texture;
	_material = rhs._material;

	return *this;
}
