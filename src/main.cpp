//============================================================================
// Name        : hello.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <memory>

#include "dpWindow.h"
#include "dpGameSettings.h"
#include "dpGameResources.h"
#include "dpShader.h"
#include "dpShaderProgram.h"
#include "dpGameState.h"
#include "dpUtility.h"
#include "dpEntity.h"

using namespace std;

managed_ptr<dpWindow> _window;
managed_ptr<dpGameSettings> _settings;
managed_ptr<dpGameResources> _resources;
managed_ptr<dpShaderProgram> _program;
managed_ptr<dpGameState> _gameState;

GLboolean _keyState[400];

void errorHandler(){
	string error;
	while ((error = dpUtility::GetError()) != "")
		cout << error << endl;
}

void error_callback(int error, const char* description){
	dpUtility::AddError(description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (action == GLFW_PRESS)
		_keyState[_settings->Controls()[key]] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		_keyState[_settings->Controls()[key]] = GL_FALSE;
}

int main(int argc, char** argv) {
	dpUtility::DefineRootPath(argv[0]);

	ilInit();
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilEnable(IL_ORIGIN_SET);

	if (!glfwInit())
		dpUtility::AddError("GLFW library failed to initialise.");
	glfwSetErrorCallback(error_callback);

	_settings.attach(new dpGameSettings());
	_program.attach(new dpShaderProgram());
	_resources.attach(new dpGameResources(_program.ptr()));
	_gameState.attach(new dpGameState());
	_window.attach(new dpWindow("Test", _settings.ptr(), _resources.ptr(), _program.ptr(), _gameState.ptr()));
	_window->SetKeyCallback(key_callback);

	cout << glGetString(GL_VERSION) << endl;
	dpBlock block(_resources->GetBlock("error"), 0);
	block.GetOrientationPtr()->Translate(glm::vec3(0, 0.5f, -2.0f));
	_gameState->GetEntities().push_back(&block);

	dpBlock block2(_resources->GetBlock("error"), 0);
	block2.GetOrientationPtr()->Translate(glm::vec3(2.0f, 0.5f, 0));
	dpUtility::GetPlayer().SetReference(block2.GetOrientationPtr());
	dpUtility::GetCamera().SetReference(block2.GetOrientationPtr());
	_gameState->GetEntities().push_back(&block2);

	dpBlock block3(_resources->GetBlock("error"), 0);
	block3.GetOrientationPtr()->Translate(glm::vec3(-2.0f, 0.5f, 0));
	_gameState->GetEntities().push_back(&block3);

	dpBlock plain(_resources->GetBlock("plain"), 0);
	_gameState->GetEntities().push_back(&plain);

	_window->UpdateEvents();
	glfwSetCursorPos(_window->Window(), (GLdouble)_settings->Window().Width() / 2, (GLdouble)_settings->Window().Height() / 2);

	while (_window->State() != glWndEnum::quit && _window->State() != glWndEnum::terminate){
		_window->UpdateEvents();

		if (_window->State() == glWndEnum::active){
			if (_keyState[GLFW_KEY_ESCAPE])
				_window->State() = glWndEnum::quit;

			if (glfwGetMouseButton(_window->Window(), GLFW_MOUSE_BUTTON_RIGHT)){
				if (_keyState[GLFW_KEY_W])
					dpUtility::GetCamera().TranslateZ(0.03f);
				if (_keyState[GLFW_KEY_S])
					dpUtility::GetCamera().TranslateZ(-0.03f);

				glm::vec2 difference = _window->ResetCursorPos();
				if (difference.x != 0){
					dpUtility::GetCamera().RotateY(difference.x * 0.1f);
				}
				if (difference.y != 0){
					dpUtility::GetCamera().RotateX(difference.y * 0.1f);
				}
			}
			else{
				if (_keyState[GLFW_KEY_D])
					dpUtility::GetPlayer().TranslateX(0.05f);
				if (_keyState[GLFW_KEY_A])
					dpUtility::GetPlayer().TranslateX(-0.05f);
				if (_keyState[GLFW_KEY_LEFT_SHIFT])
					dpUtility::GetPlayer().TranslateY(0.05f);
				if (_keyState[GLFW_KEY_LEFT_CONTROL])
					dpUtility::GetPlayer().TranslateY(-0.05f);
				if (_keyState[GLFW_KEY_W])
					dpUtility::GetPlayer().TranslateZ(-0.05f);
				if (_keyState[GLFW_KEY_S])
					dpUtility::GetPlayer().TranslateZ(0.05f);

				if (_keyState[GLFW_KEY_Q])
					dpUtility::GetPlayer().RotateZ(0.05f);
				if (_keyState[GLFW_KEY_E])
					dpUtility::GetPlayer().RotateZ(-0.05f);


				glm::vec2 difference = -_window->ResetCursorPos();
				if (difference.x != 0){
					dpUtility::GetPlayer().RotateY(difference.x * 0.1f);
				}
				if (difference.y != 0){
					dpUtility::GetPlayer().RotateX(difference.y * 0.1f);
				}
			}

			_window->NewFrame();
			errorHandler();
		}
	}

	if (_window->State() == glWndEnum::terminate){

	}

	glfwTerminate();
	return 0;
}
