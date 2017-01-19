/*
 * glWindow.cpp
 *
 *  Created on: 14 Jul 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "dpWindow.h"
#include "dpGameSettings.h"
#include "dpGameResources.h"
#include "dpShaderProgram.h"
#include "dpShader.h"

using namespace glWndEnum;
using namespace dpShaderProgramEnums;
using namespace glm;

#if (1) // glWindow class
GLvoid dpWindow::DrawObjects(){

}

GLvoid dpWindow::InitWindow(){
#if (1) //Initialise Window
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	GLFWmonitor* monitor = NULL;
	if (_settings->Window().Fullscreen() && _settings->Window().Maximised())
		monitor = glfwGetPrimaryMonitor();

	GLFWwindow* tmp;
	if (!(tmp = glfwCreateWindow(_settings->Window().Width(), _settings->Window().Height(), _title, monitor, NULL)))
		dpUtility::AddError("GLFW could not create a child window.");
	else
		glfwMakeContextCurrent(tmp);

	_active = tmp;
	if(_settings->Window().Fullscreen() && _settings->Window().Maximised()){
		_restored = tmp;
		_state = active;
		glfwRestoreWindow(_active);
	}
	else if(_settings->Window().Fullscreen() && !_settings->Window().Maximised()){
		_minimised = tmp;
		_state = paused;
		glfwIconifyWindow(_active);
	}
#endif
#if (1) //Initialise OpenGL
	glewExperimental = GL_TRUE;
	if (GLenum err = glewInit() != GLEW_OK)
		dpUtility::AddError((GLchar*)glewGetErrorString(err));

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);                         // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                    // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                     // The Type Of Depth Test To Do

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Really Nice Perspective Calculations
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
#if (1) //Initialise ShaderProgram
	managed_ptr<dpShader> vertex(new shVertex("shader.vs"));
	managed_ptr<dpShader> fragment(new shFragment("shader.fs"));

	_program->Create();
	_program->AttachShader(vertex.ptr());
	_program->AttachShader(fragment.ptr());
	_program->BindFragmentData(0, "final_colour");
	_program->Link();
	_program->Use();
#endif
#if (1) //Initialise View
    GLint width, height;
	glfwGetFramebufferSize(_active, &width, &height);
	if (width == 0 || height == 0){
        return;
    }

	GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
	mat4 projection = perspective(60.0f * 3.14f / 180, aspectRatio, 0.01f, 1000.0f);
	glUniformMatrix4fv(_program->GetVariablePos(vertProjectionTransform), 1 , false, value_ptr(projection));
#endif

	_resources->Load(_program);

	glUniform3f(_program->GetVariablePos(fragLightPos), 0.0f, 2.0f, 0.0f);
	glUniform3f(_program->GetVariablePos(fragLightCol), 1.0f, 1.0f, 1.0f);
	glUniform1f(_program->GetVariablePos(fragLightAtt), 0.0f);
	glUniform1f(_program->GetVariablePos(fragLightAmb), 0.005f);
}

dpWindow::dpWindow(const GLchar* title, dpGameSettings* settings, dpGameResources* resources, dpShaderProgram* program, dpGameState* state)
:_cursorPos((GLdouble)settings->Window().Width() / 2, (GLdouble)settings->Window().Height() / 2)
{
	_state = active;		_title = title;
	_settings = settings; 	_resources = resources;
	_program = program;		_gameState = state;

	_restored = _minimised = NULL;

	InitWindow();
}
dpWindow::~dpWindow() {
	glfwMakeContextCurrent(NULL);
	glfwDestroyWindow(_restored);
	glfwDestroyWindow(_minimised);
}
GLvoid dpWindow::SetTitle(const GLchar* title){
	_title = title;
	glfwSetWindowTitle(_active, _title);
}
GLvoid dpWindow::SetKeyCallback(GLFWkeyfun callback){
	_keyCallback = callback;
	glfwSetKeyCallback(_active, _keyCallback);
}
GLvoid dpWindow::UpdateEvents(){
	glfwPollEvents();
//	if (_settings->Updated()){
//		glfwMakeContextCurrent(NULL);
//		glfwDestroyWindow(_active);
//		_restored = NULL;
//		_minimised = NULL;
//		InitWindow();
//		SetKeyCallback(_keyCallback);
//		_settings->Updated() = false;
//	}
//	else if (_settings->Window().Fullscreen()){
//		if (_restored != NULL){
//			if (!glfwGetWindowAttrib(_restored, GLFW_FOCUSED)){
//				glfwMakeContextCurrent(NULL);
//				glfwDestroyWindow(_restored);
//				_restored = NULL;
//				_settings->Window().SwapMode();
//				InitWindow();
//			}
//		}
//
//		if (_minimised != NULL){
//			if (glfwGetWindowAttrib(_minimised, GLFW_FOCUSED)){
//				glfwMakeContextCurrent(NULL);
//				glfwDestroyWindow(_minimised);
//				_minimised = NULL;
//				_settings->Window().SwapMode();
//				InitWindow();
//				SetKeyCallback(_keyCallback);
//			}
//		}
//	}

	if (glfwWindowShouldClose(_active))
		_state = quit;
}

GLvoid dpWindow::NewFrame(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 camera = dpUtility::GetCamera().GetMatrix();
	glUniformMatrix4fv(_program->GetVariablePos(vertCameraTransform), 1 , false, value_ptr(camera));
	glUniformMatrix4fv(_program->GetVariablePos(fragCamPosition), 1 , false, value_ptr(inverse(camera)));

	vector<dpEntity*>& entities = _gameState->GetEntities();
	for (GLuint i = 0; i < entities.size(); ++i) {
		dpBlock* block = dynamic_cast<dpBlock*>(entities[i]);
		if (block){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, block->GetTexture());

			glUniform1i(_program->GetVariablePos(fragTexture), 0);

			glUniformMatrix4fv(_program->GetVariablePos(vertWorldTransform), 1, false, value_ptr(block->GetMatrix()));
			glUniformMatrix4fv(_program->GetVariablePos(fragWorldTransform), 1 , false, value_ptr(block->GetMatrix()));

			glm::vec3 specular(block->GetMaterial().GetSpecularCol());
			glm::vec4 surface(block->GetMaterial().GetSurfaceCol());

			glUniform1f(_program->GetVariablePos(fragSpecularShi), block->GetMaterial().GetSharpness());
			glUniform3f(_program->GetVariablePos(fragSpecularCol), specular.x, specular.y, specular.z);
			glUniform4f(_program->GetVariablePos(fragColour), surface.x, surface.y, surface.z, surface.w);

			glBindVertexArray(block->GetObjects().vertexArrayObject);
			glDrawElements(GL_TRIANGLES, block->GetObjects().vertexCount, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(dpNULL);

			glBindTexture(GL_TEXTURE_2D, dpNULL);
		}
	}

	glfwSwapBuffers(_active);
}
GLFWwindow* dpWindow::Window(){
	return _active;
}
glWndEnum::State& dpWindow::State(){
	return _state;
}
glm::vec2 dpWindow::ResetCursorPos(){
	GLdouble cursorX = 0, cursorY = 0;
	glfwGetCursorPos(_active, &cursorX, &cursorY);
	glfwSetCursorPos(_active, (GLdouble)_settings->Window().Width() / 2, (GLdouble)_settings->Window().Height() / 2);

	glm::vec2 newPos(cursorX, cursorY);
	glm::vec2 diff = newPos - _cursorPos;

	return diff;
}
#endif
