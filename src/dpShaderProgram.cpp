/*
 * glProgram.cpp
 *
 *  Created on: 6 Aug 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpShaderProgram.h"
#include "dpShader.h"
#include "dpUtility.h"

using namespace dpShaderProgramEnums;

dpShaderProgram::dpShaderProgram() {
	_program_id = 0;

	_variables = new GLint[50];
}
dpShaderProgram::~dpShaderProgram() {
	delete[] _variables;
	glDeleteProgram(_program_id);
}

GLvoid dpShaderProgram::Create(){
	if (_program_id == 0)
		_program_id = glCreateProgram();
	else{
		glDeleteProgram(_program_id);
		_program_id = glCreateProgram();
	}
}
GLvoid dpShaderProgram::AttachShader(dpShader* shader){
	glAttachShader(_program_id, shader->GetShaderID());
}
GLvoid dpShaderProgram::BindFragmentData(GLuint attachment, string name){
	glBindFragDataLocation(_program_id, attachment, name.c_str());
}
GLint dpShaderProgram::GetInputVarLoc(string name){
	return glGetAttribLocation(_program_id, name.c_str());
}
GLint dpShaderProgram::GetUniformVarLoc(string name){
	return glGetUniformLocation(_program_id, name.c_str());
}
GLvoid dpShaderProgram::Link(){
	glLinkProgram(_program_id);

	GLint linkStatus;
	glGetProgramiv(_program_id, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != true){
		dpUtility::AddError("Program Link Failed.");

		GLint infoLogLenght;
		glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &infoLogLenght);
		GLchar* infoLog = new GLchar[infoLogLenght + 1];
		glGetProgramInfoLog(_program_id, infoLogLenght + 1, NULL, infoLog);

		string err = infoLog;
		delete infoLog;
		dpUtility::AddError(err);
	}
}
GLvoid dpShaderProgram::Use(){
	glUseProgram(_program_id);

	_variables[vertProjectionTransform] = GetUniformVarLoc("vert_projection");
	_variables[vertCameraTransform] = GetUniformVarLoc("vert_camera");
	_variables[vertWorldTransform] = GetUniformVarLoc("vert_world");

	_variables[fragColour] = GetUniformVarLoc("frag_colour");
	_variables[fragTexture] = GetUniformVarLoc("frag_texture");
	_variables[fragCamPosition] = GetUniformVarLoc("frag_camera_transform");
	_variables[fragWorldTransform] = GetUniformVarLoc("frag_world_transform");
	_variables[fragLightPos] = GetUniformVarLoc("frag_light_params.position");
	_variables[fragLightCol] = GetUniformVarLoc("frag_light_params.colour");
	_variables[fragLightAtt] = GetUniformVarLoc("frag_light_params.attenuation");
	_variables[fragLightAmb] = GetUniformVarLoc("frag_light_params.ambient");
	_variables[fragSpecularShi] = GetUniformVarLoc("frag_reflection_params.shininess");
	_variables[fragSpecularCol] = GetUniformVarLoc("frag_reflection_params.colour");
}

GLuint dpShaderProgram::GetProgramID(){
	return _program_id;
}

GLint dpShaderProgram::GetVariablePos(int var){
	Variables vars = static_cast<Variables>(var);

	switch (vars) {
		case vertProjectionTransform:
			return _variables[vertProjectionTransform];
			break;
		case vertCameraTransform:
			return _variables[vertCameraTransform];
			break;
		case vertWorldTransform:
			return _variables[vertWorldTransform];
			break;
		case fragColour:
			return _variables[fragColour];
			break;
		case fragTexture:
			return _variables[fragTexture];
			break;
		case fragCamPosition:
			return _variables[fragCamPosition];
			break;
		case fragWorldTransform:
			return _variables[fragWorldTransform];
			break;
		case fragLightPos:
			return _variables[fragLightPos];
			break;
		case fragLightCol:
			return _variables[fragLightCol];
			break;
		case fragLightAtt:
			return _variables[fragLightAtt];
			break;
		case fragLightAmb:
			return _variables[fragLightAmb];
			break;
		case fragSpecularShi:
			return _variables[fragSpecularShi];
			break;
		case fragSpecularCol:
			return _variables[fragSpecularCol];
			break;
	}
	return 0;
}
