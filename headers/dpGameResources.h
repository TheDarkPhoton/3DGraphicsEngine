/*
 * glShaderObject.h
 *
 *  Created on: 30 Jul 2014
 *      Author: darkphoton
 */

#ifndef GLSHADEROBJECT_H_
#define GLSHADEROBJECT_H_

#include <string>
#include <vector>
#include "dpBlockAsset.h"
#include "dpMaterial.h"

class dpShaderProgram;

using namespace std;

enum dpConstants{
	error = 0,
};

class dpGameResources {
	dpShaderProgram* _program;

	vector<dpMaterial> _materialList;
	vector<dpBlockAsset> _blockList;

	GLvoid LoadFolderList(vector<string>& collection, string path);

	GLvoid LoadMaterials();

	GLboolean LoadBlockData(vector<dpModelData>& data, GLuint& texture_out, string& material_out, string path);
	GLvoid InitialiseBlocks(vector<string>& files);

public:
	dpGameResources(dpShaderProgram* program);
	~dpGameResources();

	GLvoid Load(dpShaderProgram* program);

	dpBlockAsset& GetBlock(string blockName);
	dpMaterial& GetMaterial(string materialName);
};

#endif /* GLSHADEROBJECT_H_ */
