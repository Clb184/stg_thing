#include "GameWindow.hpp"
#include "Output.h"

GameWindow::GameWindow() {
	m_WindowData = { };
}

GameWindow::~GameWindow() {

}

bool GameWindow::CreateGameWindow(const ConfigCtrl& cfg) {
	LOG_INFO("Creating Game Window");
	int width = 800, height = 80; // Default resolution just in case
	bool fullscreen = false;

	// Get resolution from value, using a formula
	int resolution = cfg.GetWindowResolution();
	resolution = (resolution > 0) ? resolution : 1;
	int baseval = 160 + 40 * resolution;
	height = baseval * 3;
	width = baseval * 4;
	fprintf(stdout, "Resolution: %d, Width: %d, Height: %d\n", resolution, width, height);

	if(cfg.GetWindowState() == WS_FULLSCREEN) fullscreen = true;

	if(false == CreateGLWindow("Game Window", width, height, fullscreen, &m_WindowData)) {
		fprintf(stdout, "Failed creating OpenGL window\n");
		return false;
	}
	
	m_Width = width; m_Height = height;

	return true;
}

void GameWindow::SetGameTitle(const char* title) {
	glfwSetWindowTitle(m_WindowData.window, title);
}

void GameWindow::SetWindowState(int state) {
	GLFWmonitor* monitor = nullptr;
	switch(state) {
		case WS_FULLSCREEN: monitor = glfwGetPrimaryMonitor(); break;
		default:
		case WS_WINDOW: monitor = nullptr; break;
		case WS_BORDERLESS: monitor = nullptr; break;
	}
	glfwSetWindowMonitor(m_WindowData.window, monitor, 0, 0, m_Width, m_Height, GLFW_DONT_CARE);
}

void GameWindow::DestroyWindow() {
	glfwDestroyWindow(m_WindowData.window);
}

window_t* GameWindow::GetWindowData() {
	return &m_WindowData;
}

