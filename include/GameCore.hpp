#ifndef GAMECORE_INCLUDED
#define GAMECORE_INCLUDED

#include "XPFW.h"
#include "XASM2/VM.hpp"
#include "ConfigCtrl.hpp"
#include "GameWindow.hpp"
#include "GameState.hpp"
#include "InputDevice.hpp"

class GameCore {
public:
	GameCore();
	~GameCore();

	bool InitGame();
	void StopGame();

	void Move(float dt);
	void Draw(float dt);
	
	void SetWindowTitle(const char* title);
	ConfigCtrl* GetConfigCtrl();

private:
	ConfigCtrl m_CFG;
	InputDevice m_Input;
	GameWindow m_Window;
	GameState m_State;
};

#endif
