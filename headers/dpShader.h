/*
 * glShader.h
 *
 *  Created on: 6 Aug 2014
 *      Author: darkphoton
 */

#ifndef GLSHADER_H_
#define GLSHADER_H_

#include <string>

using namespace std;

class dpShader {
	GLuint _shader_id;

	GLvoid LoadShader(string& source);
public:
	dpShader(string path, GLenum type);
	virtual ~dpShader();

	GLuint GetShaderID();
};

class shVertex : public dpShader{
public:
	shVertex(string path);
};

class shFragment : public dpShader{
public:
	shFragment(string path);
};
#endif /* GLSHADER_H_ */
