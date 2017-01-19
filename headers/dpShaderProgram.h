/*
 * glProgram.h
 *
 *  Created on: 6 Aug 2014
 *      Author: darkphoton
 */

#ifndef GLPROGRAM_H_
#define GLPROGRAM_H_

#include <string>

using namespace std;

class dpShader;

class dpShaderProgram {
	GLuint _program_id;

	GLint* _variables;

public:
	dpShaderProgram();
	~dpShaderProgram();

	GLvoid Create();
	GLvoid AttachShader(dpShader* shader);
	GLvoid BindFragmentData(GLuint attachment, string name);
	GLint GetInputVarLoc(string name);
	GLint GetUniformVarLoc(string name);
	GLvoid Link();
	GLvoid Use();

	GLuint GetProgramID();
	GLint GetVariablePos(int var);
};

#endif /* GLPROGRAM_H_ */
