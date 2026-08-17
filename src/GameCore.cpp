#include "GameCore.hpp"
#include "Output.h"

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
	m_IO.Init();

	// Load game's descriptor
	if(false == m_Info.Init()) {
		LOG_ERROR("Failed initializing GameInfo");
		return false;
	}
	LoadGameInfo();

	// Initialize GameState, else it will fail
	if(false == m_State.Init(&m_Input, &m_Info, &m_IO)) {
		LOG_ERROR("Failed initializing GameState\n");
		return false;
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
	m_Input.Update();
	m_State.Move(dt);
	m_IO.Move(dt);
	if(m_Input.GetScreenshot()) {
		
	}
}

void GameCore::Draw(float dt) {
	m_State.Draw();
	m_IO.Draw();
}

void GameCore::SetWindowTitle(const char* title) {
	m_Window.SetTitle(title);
}

ConfigCtrl* GameCore::GetConfigCtrl() {
	return &m_CFG;
}

void GameCore::LoadGameInfo() {
			
	// Start with the properties
	m_Window.SetTitle(m_Info.GetTitle().c_str()); // Title
	if(false == InitializeSoundControl(&g_Sound, m_Info.GetSoundEntries().size())) { // Sounds
		m_IO.LogError("Failed loading sounds");
	} else {
		m_IO.LogInfo("SoundControl initialized");
		//m_IO.LogInfo("Loaded " + std::to_string(sounds.size()) + " sounds");
	}
}
