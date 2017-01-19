/*
 * glSettings.h
 *
 *  Created on: 21 Jul 2014
 *      Author: darkphoton
 */

#ifndef GLSETTINGS_H_
#define GLSETTINGS_H_

#include <string>
using namespace std;

class dpWindowSettings {
	GLboolean _fullscreen;
	GLboolean _visible;
	GLint _width;
	GLint _height;

public:
	dpWindowSettings();

	GLboolean& Fullscreen();
	GLboolean Maximised();
	GLvoid SwapMode();
	GLint& Width();
	GLint& Height();
};

class dpSoundSettings{
	GLint _overall;
	GLint _music;
	GLint _effects;
	GLint _dialog;

public:
	dpSoundSettings();

	GLint& Overall();
	GLint& Music();
	GLint& Effects();
	GLint& Dialog();
};

class dpGraphicSettings{

};

class dpControlSettings{
	class dpKey{
		GLboolean _active;
		GLint _key;
	public:
		dpKey();
		GLint get_Key(GLint systemKey);
		GLvoid set_Key(GLint customKey);
	};
	dpKey _keys[400];

public:
	GLvoid set_Key(GLint systemKey, GLint customKey);
	GLint operator[](GLint key);
};

class dpGameplaySettings{

};

class dpGameSettings{
	string _settingsPath;
	string _settingsFile;
	GLboolean _updated;

	dpWindowSettings _window;
	dpSoundSettings _sound;
	dpGraphicSettings _graphics;
	dpControlSettings _controls;
	dpGameplaySettings _game;

public:
	dpGameSettings();
	GLvoid Load();
	GLvoid Save();

	dpWindowSettings& Window();
	dpGraphicSettings& Graphics();
	dpControlSettings& Controls();
	GLboolean& Updated();
};

#endif /* GLSETTINGS_H_ */
