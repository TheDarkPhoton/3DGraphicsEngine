/*
 * glWindow.h
 *
 *  Created on: 14 Jul 2014
 *      Author: darkphoton
 */

#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#include <glm/glm.hpp>
#include <vector>
#include "dpGameSettings.h"
#include "dpGameResources.h"
#include "dpShaderProgram.h"
#include "dpGameState.h"
#include "dpUtility.h"

using namespace std;

namespace glWndEnum {
	enum State{
		active = 0, paused = 1, minimised = 2, quit = 3, terminate = 4
	};
}

class dpWindow {
	GLFWwindow* _active;
	GLFWwindow* _restored;
	GLFWwindow* _minimised;
	dpGameSettings* _settings;
	dpGameResources* _resources;
	dpShaderProgram* _program;
	dpGameState* _gameState;

	GLFWkeyfun _keyCallback;

	const GLchar* _title;
	glWndEnum::State _state;
	glm::vec2 _cursorPos;

	GLvoid InitWindow();
	GLvoid DrawObjects();

public:
	dpWindow(const GLchar* title, dpGameSettings* settings, dpGameResources* resources,
			dpShaderProgram* program, dpGameState* state);
	~dpWindow();

	GLvoid SetTitle(const GLchar* title);
	GLvoid SetKeyCallback(GLFWkeyfun callback);

	GLvoid UpdateEvents();
	GLvoid NewFrame();

	GLFWwindow* Window();
	glWndEnum::State& State();
	glm::vec2 ResetCursorPos();
};

#endif /* GLWINDOW_H_ */
