/*
 * glSettings.cpp
 *
 *  Created on: 21 Jul 2014
 *      Author: darkphoton
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "dpGameSettings.h"
#include "dpUtility.h"

#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#if (1) //WindowSettings
dpWindowSettings::dpWindowSettings(){
	_fullscreen = false;
	_visible = true;
	_width = 800;
	_height = 600;
}
GLboolean& dpWindowSettings::Fullscreen(){
	return _fullscreen;
}
GLboolean dpWindowSettings::Maximised(){
	return _visible;
}
GLvoid dpWindowSettings::SwapMode(){
	_visible = !_visible;
}
GLint& dpWindowSettings::Width(){
	return _width;
}
GLint& dpWindowSettings::Height(){
	return _height;
}
#endif
#if (1) //glSoundSettings
dpSoundSettings::dpSoundSettings(){
	_overall = 100;
	_music = 100;
	_effects = 100;
	_dialog = 100;
}

GLint& dpSoundSettings::Overall(){
	return _overall;
}
GLint& dpSoundSettings::Music(){
	return _music;
}
GLint& dpSoundSettings::Effects(){
	return _effects;
}
GLint& dpSoundSettings::Dialog(){
	return _dialog;
}
#endif
#if (1) //glGraphicSettings

#endif
#if (1) //glControlSettings
	dpControlSettings::dpKey::dpKey(){
		_active = false;
		_key = 0;
	}
	GLint dpControlSettings::dpKey::get_Key(GLint systemKey){
		if (!_active){
			_key = systemKey;
			_active = true;
		}

		return _key;
	}
	GLvoid dpControlSettings::dpKey::set_Key(GLint customKey){
		_key = customKey;
		_active = true;
	}

	GLint dpControlSettings::operator[](GLint key){
		return _keys[key].get_Key(key);
	}
	GLvoid dpControlSettings::set_Key(GLint systemKey, GLint customKey){
		_keys[systemKey].set_Key(customKey);
	}
#endif

dpGameSettings::dpGameSettings(){
	_updated = false;

	_settingsPath = dpUtility::GetRootPath();
	_settingsFile = "/settings.txt";

	Load();
}
GLvoid dpGameSettings::Load(){
	string file = _settingsPath + _settingsFile;
	ifstream infile(file.c_str());

	if (!infile)
		Save();
	else{
		string line;
		while (getline(infile, line)){
			istringstream issLine(line);

			string param;
			if (issLine >> param){
				vector<string> values;
				string tmp;
				while (issLine >> tmp){
					values.push_back(tmp);
				}

				if (param == "fullscreen"){
					if (values[0].substr(0, values[0].size() - 1) == "true")
						_window.Fullscreen() = true;
					else
						_window.Fullscreen() = false;
				}
				else if (param == "resolution"){
					istringstream issWidth(values[0]);
					istringstream issHeight(values[1]);

					issWidth >> _window.Width();
					issHeight >> _window.Height();
				}
				else if (param == "sound_overall"){
					istringstream issSound(values[0]);
					issSound >> _sound.Overall();
				}
				else if (param == "sound_music"){
					istringstream issSound(values[0]);
					issSound >> _sound.Music();
				}
				else if (param == "sound_effects"){
					istringstream issSound(values[0]);
					issSound >> _sound.Effects();
				}
				else if (param == "sound_dialog"){
					istringstream issSound(values[0]);
					issSound >> _sound.Dialog();
				}
				else if (param == "controls"){
					for (GLint i = 0; i < 400; i += 2) {
						GLint systemKey, customKey;
						istringstream issControls1(values[i]); issControls1 >> systemKey;
						istringstream issControls2(values[i + 1]); issControls2 >> customKey;
						_controls.set_Key(systemKey, customKey);
					}
				}
			}
		}
	}
	infile.close();
}
GLvoid dpGameSettings::Save(){
	string file = _settingsPath + _settingsFile;
	ofstream outstream(file.c_str());

	if (_window.Fullscreen() == true)
		outstream << "fullscreen true;" << endl;
	else
		outstream << "fullscreen false;" << endl;

	outstream << "resolution " << _window.Width() << " " << _window.Height() << ";" << endl;
	outstream << "sound_overall " << _sound.Overall() << ";" << endl;
	outstream << "sound_music " << _sound.Music() << ";" << endl;
	outstream << "sound_effects " << _sound.Effects() << ";" << endl;
	outstream << "sound_dialog " << _sound.Dialog() << ";" << endl;

	outstream << "controls ";
	for (GLint i = 0; i < 400; i++) {
		outstream << i << " " << _controls[i] << "; ";
	}
	outstream << endl;

	outstream.close();
}

dpWindowSettings& dpGameSettings::Window(){
	return _window;
}
dpGraphicSettings& dpGameSettings::Graphics(){
	return _graphics;
}
dpControlSettings& dpGameSettings::Controls(){
	return _controls;
}
GLboolean& dpGameSettings::Updated(){
	return _updated;
}
