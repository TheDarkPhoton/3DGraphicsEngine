/*
 * Utility.h
 *
 *  Created on: 29 Jul 2014
 *      Author: darkphoton
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>
#include "dpEntity.h"
using namespace std;

const GLint dpNULL = 0;

namespace dpShaderProgramEnums {
	enum Variables {
		fragColour = 0,
		fragTexture = 1,
		fragCamPosition = 2,
		fragWorldTransform = 3,
		fragLightPos = 4,
		fragLightCol = 5,
		fragLightAtt = 6,
		fragLightAmb = 7,
		fragSpecularShi = 8,
		fragSpecularCol = 9,
		vertProjectionTransform = 10,
		vertCameraTransform = 11,
		vertWorldTransform = 12,
	};
}  // namespace ShaderVariables

struct Light{
	glm::vec3 position;
	glm::vec3 intensities;
};

class dpUtility {
	static vector<string> errors;
	static string rootPath;
	static dpCamera camera;
	static dpPlayer player;
	static Light testLight;

public:
	static dpCamera& GetCamera();
	static dpPlayer& GetPlayer();

	static void DefineRootPath(const GLchar* appPath);
	static string GetRootPath();

	static GLvoid AddError(const GLchar* error);
	static GLvoid AddError(string error);
	static string GetError();

	static void ReplaceString(string& subject, const string& search, const string& replace);

	static string LoadShaderData(const GLchar* shader);
};

template <class T>
class managed_ptr{
	T* _class_ptr;

public:
	managed_ptr(T* class_ptr){
		_class_ptr = class_ptr;
	}
	managed_ptr(){
		_class_ptr = NULL;
	}
	~managed_ptr(){
		if (_class_ptr != NULL)
			delete _class_ptr;
	}

	GLvoid attach(T* class_ptr){
		if (_class_ptr == NULL)
			_class_ptr = class_ptr;
		else{
			delete _class_ptr;
			_class_ptr = class_ptr;
		}
	}

	T* ptr(){
		return _class_ptr;
	}
	T* operator->(){
		return _class_ptr;
	}
};

#endif /* UTILITY_H_ */
