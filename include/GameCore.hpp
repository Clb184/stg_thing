#ifndef GAMECORE_INCLUDED
#define GAMECORE_INCLUDED

#include "XPFW.h"
#include "XASM2/VM.hpp"
#include "ConfigCtrl.hpp"
#include "GameWindow.hpp"
#include "GameState.hpp"

class GameCore {
public:
	GameCore();
	~GameCore();

	bool InitGame();
	void StopGame();

	void Move(float dt);
	void Draw(float dt);
private:
	ConfigCtrl m_CFG;
	GameWindow m_Window;
	GameState m_State;
};

#endif
