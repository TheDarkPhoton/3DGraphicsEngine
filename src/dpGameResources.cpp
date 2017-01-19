/*
 * glShaderObject.cpp
 *
 *  Created on: 30 Jul 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <iterator>

#include "dpGameResources.h"
#include "dpShader.h"
#include "dpShaderProgram.h"
#include "dpUtility.h"

using namespace boost::filesystem;

GLvoid dpGameResources::LoadFolderList(vector<string>& collection, string path){
	boost::filesystem::path folderPath = path.c_str();

	vector<string> folders;
	if (exists(folderPath.c_str())){
		typedef vector<boost::filesystem::path> paths;

		if (is_regular_file(folderPath)){
			collection.push_back(string(folderPath.c_str()));
		}
		else if (is_directory(folderPath)){
			paths p;
			copy(directory_iterator(folderPath), directory_iterator(), back_inserter(p));

			for (GLuint i = 0; i < p.size(); ++i){
				string tmp(p[i].c_str());
				folders.push_back(tmp);
			}
		}
	}

	for (GLuint i = 0; i < folders.size(); ++i) {
		LoadFolderList(collection, folders[i]);
	}
}

GLboolean dpGameResources::LoadBlockData(vector<dpModelData>& data, GLuint& texture_out, string& material_out, string path){
	GLboolean vertex, index;
	GLuint tex = true;
	GLuint counter = 0;
	string texture_name;

#if (1) // Load All Vertex Data
	vector<string> lines;

	//string dataPath = dpUtility::GetRootPath() + string("/resources/blocks/") + path;
	ifstream verticesFile(path.c_str());
	if (verticesFile){
		string line;

		while (getline(verticesFile, line)){
			lines.push_back(line);
		}
	}
	verticesFile.close();

	vector<string> stack;
	dpModelData objects;

	GLboolean param = true;
	for (GLuint l = 0; l < lines.size(); ++l) {
		istringstream issLine(lines[l]);

		string word;

		if (lines[l].find(string("}")) != std::string::npos)
			param = true;

		if (param)
			issLine >> word;

		if (!param){
			string last_word;
			if (!stack.empty())
				last_word = stack.back();

			if (last_word == "vertices"){
				GLfloat vertVal;
				while (issLine >> vertVal){
					objects.vertices.push_back(vertVal);
					counter++;
				}
			}
			else if (last_word == "indices"){
				GLuint vertVal;
				while (issLine >> vertVal){
					objects.indices.push_back(vertVal);
					counter++;
				}
			}
			else if (last_word == "properties"){
				string prop, value;
				issLine >> prop >> value;
				if(prop == "texture"){
					texture_name = value;
				}
				else if(prop == "material"){
					material_out = value;
				}
			}
		}
		else if (word == "block"){
			issLine >> word;
			if (word == "{") {
				dpModelData tmp; objects = tmp;
				stack.push_back("block");
			}
		}
		else if(word == "vertices") {
			issLine >> word;
			if (stack.back() == "block" && word == "{") {
				stack.push_back("vertices");
				param = false;
			}
		}
		else if(word == "indices") {
			issLine >> word;
			if (stack.back() == "block" && word == "{") {
				stack.push_back("indices");
				param = false;
			}
		}
		else if(word == "properties") {
			issLine >> word;
			if (word == "{") {
				stack.push_back("properties");
				param = false;
			}
		}
		else if (word == "}"){
			if (!stack.empty()){
				if (stack.back() == "block"){
					data.push_back(objects);
				}
				else if (stack.back() == "vertices"){
					if (counter % 8 == 0 && counter / 8 >= 3)
						vertex = true;
					else
						dpUtility::AddError("Error while loading vertices found");
				}
				else if (stack.back() == "indices"){
					if (counter % 3 == 0 && counter > 0)
						index = true;
					else
						dpUtility::AddError("Error while loading indices found");
				}

				stack.pop_back();
			}
		}
	}
#endif
#if (1) // Load Texture
	ILuint image = ilGenImage();
	ilBindImage(image);

	string texturePath = dpUtility::GetRootPath() + string("/resources/textures/") + texture_name;
	ILboolean loaded = ilLoadImage(texturePath.c_str());

	if (!loaded){
		dpUtility::AddError("Texture failed to Load");
		tex = false;
	}

	ILboolean converted = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	if (!converted){
		dpUtility::AddError("Texture was not converted");
		tex = false;
	}

	iluGammaCorrect(1.0f/2.2f);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
			ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

	glBindTexture(GL_TEXTURE_2D, dpNULL);
	ilBindImage(dpNULL);
	ilDeleteImage(image);

	texture_out = texture;
#endif

	if (vertex && index && tex && material_out != "")
		return true;
	else
		return false;
}

GLvoid dpGameResources::InitialiseBlocks(vector<string>& files){
	for (GLuint f = 0; f < files.size(); ++f) {
		GLuint texture;
		string material_name;
		vector<dpModelData> blockData;
		if (LoadBlockData(blockData, texture, material_name, files[f])){
			vector<dpModelResources> models;
			for (GLuint i = 0; i < blockData.size(); ++i) {
				dpModelResources objects;
				vector<GLfloat> vBuffer = blockData[i].vertices;
				vector<GLuint> iBuffer = blockData[i].indices;

				GLuint vertexArrayObject;
				glGenVertexArrays(1, &vertexArrayObject);
				glBindVertexArray(vertexArrayObject);

				GLuint vertexBufferObject;
				glGenBuffers(1, &vertexBufferObject);
				glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
				glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), &vBuffer[0], GL_STATIC_DRAW);

				GLint positionLoc = _program->GetInputVarLoc("vert_position");
				GLint texCoordLoc = _program->GetInputVarLoc("vert_texture_coordinates");
				GLint normalLoc = _program->GetInputVarLoc("vert_normal");

				glEnableVertexAttribArray(positionLoc);
				glEnableVertexAttribArray(texCoordLoc);
				glEnableVertexAttribArray(normalLoc);

				glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(0*sizeof(GLfloat)));
				glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
				glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));

				GLuint elementBufferObject;
				glGenBuffers(1, &elementBufferObject);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, iBuffer.size() * sizeof(GLuint), &iBuffer[0], GL_STATIC_DRAW);

				glBindVertexArray(dpNULL);

				objects.vertexCount = blockData[i].indices.size();
				objects.vertexBufferObject = vertexBufferObject;
				objects.elementBufferObject = elementBufferObject;
				objects.vertexArrayObject = vertexArrayObject;

				models.push_back(objects);
			}

			GLuint start = files[f].find_last_of("/\\") + 1;
			GLuint end = files[f].find_last_of(".") - start;
			string name = files[f].substr(start, end);

			dpBlockAsset block(name, models, texture, GetMaterial(material_name));
			_blockList.push_back(block);
		}
	}
}

dpGameResources::dpGameResources(dpShaderProgram* program){
	_program = program;

	dpMaterial ground("ground", glm::vec4(1.0f / 255 * 102, 1.0f / 255 * 51, 0, 1), 0.0f, 0.001f, glm::vec3(1, 1, 1));
	dpMaterial gold("gold", glm::vec4(1, 1, 0, 1), 1, 1, glm::vec3(1, 1, 0));

	_materialList.push_back(ground);
	_materialList.push_back(gold);
}

dpGameResources::~dpGameResources(){
	for (GLuint i = 0; i < _blockList.size(); ++i) {
		_blockList[i].Delete();
	}
}

GLvoid dpGameResources::Load(dpShaderProgram* program){
	vector<string> files;
	LoadFolderList(files, dpUtility::GetRootPath() + string("/resources/blocks/"));
	InitialiseBlocks(files);


}

dpBlockAsset& dpGameResources::GetBlock(string blockName){
	for (GLuint i = 0; i < _blockList.size(); ++i) {
		if (_blockList[i].Name() == blockName)
			return _blockList[i];
	}
	return _blockList[error];
}

dpMaterial& dpGameResources::GetMaterial(string materialName){
	for (GLuint i = 0; i < _materialList.size(); ++i) {
		if (_materialList[i].Name() == materialName)
			return _materialList[i];
	}
	return _materialList[error];
}
