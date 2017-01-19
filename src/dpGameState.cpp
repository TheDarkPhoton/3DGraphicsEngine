/*
 * dpGameState.cpp
 *
 *  Created on: 7 Aug 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpGameState.h"

dpGameState::dpGameState() {
}

dpGameState::~dpGameState() {
	for (GLuint i = 0; i < _entities.size(); ++i) {
		delete _entities[i];
	}
}

vector<dpEntity*>& dpGameState::GetEntities(){
	return _entities;
}
