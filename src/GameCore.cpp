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
	LoadGameDescriptor();

	// Initialize GameState, else it will fail
	if(false == m_State.Init(&m_Input, &m_IO)) {
		LOG_ERROR("Failed initializing GameState\n");
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
	m_Window.SetGameTitle(title);
}

ConfigCtrl* GameCore::GetConfigCtrl() {
	return &m_CFG;
}

void GameCore::LoadGameDescriptor() {
	pack_file_t pack;
	SetWindowTitle("STG Engine by Clb184");
	std::string title = "";
	std::vector<std::string> sounds;
	if(0 == PackFileOpen(&pack, "STG.DAT")) {
		LOG_INFO("Loading STG.DAT...");
		char* data;
		size_t size;
		if(0 == PackFileLoadEntry(&pack, "game.json", (void**)&data, &size)) {
			nlohmann::json jsn = nlohmann::json::parse(data);
			
			// Load game title
			if(jsn.find("title") != jsn.end()) {
				title = jsn["title"];
			} else {
				m_IO.LogError("Title string not found");
			}
			
			// Load sounds
			if(jsn.find("sounds") != jsn.end()) {
				sounds = jsn["sounds"];
			} else {
				m_IO.LogError("Sound names not found");
			}

			free(data);
		}
		else {
			m_IO.LogError("game.json not found");
			PackFileClose(&pack);
		}

		PackFileClose(&pack);
	} else {
		m_IO.LogError("Failed to load game info");
	}

	// Start with the properties
	SetWindowTitle(title.c_str()); // Title
	if(false == InitializeSoundControl(&g_Sound, sounds.size())) { // Sounds
		m_IO.LogError("Failed loading sounds");
	} else {
		m_IO.LogInfo("SoundControl initialized");
		m_IO.LogInfo("Loaded " + std::to_string(sounds.size()) + " sounds");
	}
}
