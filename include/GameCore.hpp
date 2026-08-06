#ifndef GAMECORE_INCLUDED
#define GAMECORE_INCLUDED

#include "ConfigCtrl.hpp"
#include "GameWindow.hpp"
#include "GameState.hpp"
#include "InputDevice.hpp"
#include "ScreenOutput.hpp"

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
	void LoadGameDescriptor();

private:
	ConfigCtrl m_CFG;
	InputDevice m_Input;
	ScreenOutput m_IO;
	GameWindow m_Window;
	GameState m_State;
};

#endif
