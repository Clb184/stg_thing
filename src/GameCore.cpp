#include "GameCore.hpp"

void GameCoreMove(window_t* window, float dt, void* gamecore) {
	GameCore* core = (GameCore*)gamecore;
	core->Move(dt);
}

void GameCoreDraw(window_t* window, float dt, void* gamecore) {
	GameCore* core = (GameCore*)gamecore;
	core->Draw(dt);
}

GameCore::GameCore() {

}

GameCore::~GameCore() {

}

bool GameCore::InitGame() {
	// Create window
	if(false == m_Window.CreateWindow(m_CFG)) return false;

	// Set XASM2 seed
	XASM2RandomInit(123);

	// Start running the loop
	RunMainLoopDT(m_Window.GetWindowData(), this, GameCoreMove, GameCoreDraw);
	return true;
}

void GameCore::StopGame() {
	m_Window.GetWindowData()->on_exit = true;
	m_CFG.SaveConfig();
}

void GameCore::Move(float dt) {
	fprintf(stdout, "DT move is: %.3f\n", dt);
}

void GameCore::Draw(float dt) {

}
