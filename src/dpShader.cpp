/*
 * glShader.cpp
 *
 *  Created on: 6 Aug 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpShader.h"
#include "dpUtility.h"

GLvoid dpShader::LoadShader(string& source){
	const GLchar* cSource = source.c_str();
	glShaderSource(_shader_id, 1, &cSource, NULL);
	glCompileShader(_shader_id);

	GLint compileStatus;
	glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != true){
		GLint infoLogLenght;
		glGetShaderiv(_shader_id, GL_INFO_LOG_LENGTH, &infoLogLenght);
		GLchar* infoLog = new GLchar[infoLogLenght + 1];
		glGetShaderInfoLog(_shader_id, infoLogLenght + 1, NULL, infoLog);

		string err = infoLog;
		delete infoLog;
		dpUtility::AddError(err);
	}
}

dpShader::dpShader(string path, GLenum type) {
	_shader_id = glCreateShader(type);
	string shaderData = dpUtility::LoadShaderData(path.c_str());
	LoadShader(shaderData);
}

dpShader::~dpShader() {
	glDeleteShader(_shader_id);
}

GLuint dpShader::GetShaderID(){
	return _shader_id;
}

shVertex::shVertex(string path) : dpShader(path, GL_VERTEX_SHADER) {}
shFragment::shFragment(string path) : dpShader(path, GL_FRAGMENT_SHADER) {}
