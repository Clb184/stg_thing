#ifndef GAMEWINDOW_INCLUDED
#define GAMEWINDOW_INCLUDED

#include "XPFW.h"
#include "ConfigCtrl.hpp"

class GameWindow {
public:
	GameWindow();
	~GameWindow();

	bool CreateGameWindow(const ConfigCtrl& cfg);
	void SetGameTitle(const char* title);
	void SetWindowState(int state);
	void DestroyWindow();
	window_t* GetWindowData();

private:
	window_t m_WindowData;
	int m_Width;
	int m_Height;
};

#endif
