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
	if(false == m_Window.CreateGameWindow(m_CFG)) return false;

	// Input Device
	m_Input.Init(m_Window.GetWindowData()->window);

	// Initialize GameState, else it will fail
	if(false == m_State.Init(&m_Input)) {
		fprintf(stdout, "Failed initializing GameState\n");
	}

	// Start running the loop
	RunMainLoopDT(m_Window.GetWindowData(), this, GameCoreMove, GameCoreDraw);
	m_CFG.SaveConfig();
	return true;
}

void GameCore::StopGame() {
	m_Window.GetWindowData()->on_exit = true;
}

void GameCore::Move(float dt) {
	//fprintf(stdout, "DT move is: %.3f or %.3f fps\n", dt, 1.0f / dt);
	m_State.Move(dt);
}

void GameCore::Draw(float dt) {
	m_State.Draw();
}

void GameCore::SetWindowTitle(const char* title) {
	m_Window.SetGameTitle(title);
}

ConfigCtrl* GameCore::GetConfigCtrl() {
	return &m_CFG;
}

