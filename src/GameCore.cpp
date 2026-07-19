#include "GameCore.hpp"

void GameCoreMove(window_t* window, float dt, void* gamecore) {
	GameCore* core = (GameCore*)gamecore;
	core->Move(dt);
}

void GameCoreDraw(window_t* window, float dt, void* gamecore) {
	GameCore* core = (GameCore*)gamecore;
	core->Draw(dt);
}

GameCore::GameCore() : m_State(this) {

}

GameCore::~GameCore() {

}

bool GameCore::InitGame() {
	// Create window
	if(false == m_Window.CreateWindow(m_CFG)) return false;


	// Initialize GameState, else it will fail
	if(false == m_State.Init()) {
		fprintf(stdout, "Failed initializing GameState\n");
	}

	// Start running the loop
	RunMainLoopDT(m_Window.GetWindowData(), this, GameCoreMove, GameCoreDraw);
	StopGame();
	return true;
}

void GameCore::StopGame() {
	m_Window.GetWindowData()->on_exit = true;
	m_CFG.SaveConfig();
}

void GameCore::Move(float dt) {
	//fprintf(stdout, "DT move is: %.3f or %.3f fps\n", dt, 1.0f / dt);
	m_State.Move(dt);
}

void GameCore::Draw(float dt) {
	m_State.Draw();
}
