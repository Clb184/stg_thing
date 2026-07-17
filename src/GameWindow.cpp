#include "GameWindow.hpp"
#include "Output.h"

GameWindow::GameWindow() {
	m_WindowData = { };
}

GameWindow::~GameWindow() {
}

bool GameWindow::CreateWindow(const ConfigCtrl& cfg) {
	LOG_INFO("Creating Game Window");
	int width = 800, height = 80; // Default resolution just in case
	bool fullscreen = false;

	// Get resolution from value, using a formula
	int baseval = 160 + 40 * cfg.GetWindowResolution();
	height = baseval * 3;
	width = baseval * 4;

	if(cfg.GetWindowState() == WS_FULLSCREEN) fullscreen = true;

	if(false == CreateGLWindow("Game Window", width, height, fullscreen, &m_WindowData)) {
		fprintf(stdout, "Failed creating OpenGL window\n");
		return false;
	}
	
	return true;
}

void GameWindow::SetGameTitle(const char* title) {

}

void GameWindow::SetWindowState(int state) {

}

void GameWindow::DestroyWindow() {

}

window_t* GameWindow::GetWindowData() {
	return &m_WindowData;
}

