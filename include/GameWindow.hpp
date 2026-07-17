#ifndef GAMEWINDOW_INCLUDED
#define GAMEWINDOW_INCLUDED

#include "XPFW.h"
#include "ConfigCtrl.hpp"

class GameWindow {
public:
	GameWindow();
	~GameWindow();

	bool CreateWindow(const ConfigCtrl& cfg);
	void SetGameTitle(const char* title);
	void SetWindowState(int state);
	void DestroyWindow();
	window_t* GetWindowData();

private:
	window_t m_WindowData;
};

#endif
