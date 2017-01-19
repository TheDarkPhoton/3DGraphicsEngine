/*
 * Utility.cpp
 *
 *  Created on: 29 Jul 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpUtility.h"

#include <fstream>

string dpUtility::rootPath = "";
vector<string> dpUtility::errors;
dpCamera dpUtility::camera;
dpPlayer dpUtility::player;
Light dpUtility::testLight;

dpCamera& dpUtility::GetCamera(){
	return camera;
}
dpPlayer& dpUtility::GetPlayer(){
	return player;
}

void dpUtility::ReplaceString(string& subject, const string& search, const string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void dpUtility::DefineRootPath(const GLchar* appPath){
	rootPath = appPath;
	GLint pos = rootPath.find_last_of("/\\");
	rootPath = rootPath.substr(0, pos);
	ReplaceString(rootPath, "\\" , "/");
}
string dpUtility::GetRootPath(){
	return rootPath;
}

GLvoid dpUtility::AddError(const GLchar* error){
	string strError = error;
	errors.insert(errors.begin(), strError);
}
GLvoid dpUtility::AddError(string error){
	errors.insert(errors.begin(), error);
}
string dpUtility::GetError(){
	if (errors.size() > 0){
		string strError = errors[errors.size() - 1];
		errors.pop_back();
		return strError;
	}
	else
		return "";
}

string dpUtility::LoadShaderData(const GLchar* shader){
	string fullPath = GetRootPath() + "/resources/shaders/" + shader;

	ifstream fin;
	fin.open(fullPath.c_str());

	if (!fin.is_open()){
		fin.close();
		string err = "File not found: ";
		err.append(shader);
		AddError(err);
		return (err = "");
	}

	string source((istreambuf_iterator<GLchar>(fin)), istreambuf_iterator<GLchar>());
	fin.close();

	return source;
}
