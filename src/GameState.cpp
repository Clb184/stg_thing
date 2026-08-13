#include "GameState.hpp"
#include "Output.h"
#include "cassert"
#include "GameCore.hpp"
#include "PackArchive/PackFile.h"

GameState::GameState(GameCore* core) {
	m_pCurrentScene = nullptr;
	
	// Instance all scenes
	m_pMain = new SceneMain();
	//m_pTitle = new SceneTitle();
	m_pGameMain = new SceneGameMain();
	
	// Point to Core
	m_pCore = core;
	m_pIO = nullptr;
	m_pInput = nullptr;

	m_bOnSceneChange = false;
	m_TargetSceneChange = SCENE_TITLE;
	m_CurrentSceneType = SCENE_NULL;
}

GameState::~GameState() {
	m_pCurrentScene = nullptr;
	if(nullptr != m_pMain) { 
		delete m_pMain;
		m_pMain = nullptr;
	}

	if(nullptr != m_pGameMain) {
		delete m_pGameMain;
		m_pGameMain = nullptr;
	}
}

bool GameState::Init(InputDevice* input, ScreenOutput* IO) {
	LOG_INFO("Initializing GameState");
	char buf[512] = "";
	// Check if all scenes are initialized
	if(nullptr == m_pMain || nullptr == m_pGameMain)	{
		LOG_ERROR("Failed to allocate memory for scenes:");
		sprintf(buf, "Main: %p, GameMain: %p", m_pMain, m_pGameMain);
		LOG_ERROR(buf);
		return false;
	}

	// Main Scene is the default scene
	m_pCurrentScene = m_pMain;
	m_CurrentSceneType = SCENE_MAIN;
	
	// For debug purpose
	m_pCurrentScene = m_pGameMain;
	m_CurrentSceneType = SCENE_GAMEMAIN;
	

	m_pInput = input;
	m_pIO = IO;

	if(false == m_pCurrentScene->Init(this, input, IO)) {
		LOG_ERROR("Failed initializing default scene");
		return false;
	}

	return true;
}

void GameState::Move(float dt) {
	assert(0 != m_pCurrentScene);

	// Move Scene
	m_pCurrentScene->Move(dt);
	
	// Change scene on notify
	if(m_bOnSceneChange) {
		switch(m_TargetSceneChange) {
			default: m_TargetSceneChange = SCENE_MAIN; // Default to Main title in any case
			case SCENE_MAIN: m_pCurrentScene = m_pMain;  break; // Or Main Title
			case SCENE_GAMEMAIN: m_pCurrentScene = m_pGameMain; break; // Game itself
		}

		m_bOnSceneChange = false;

		m_CurrentSceneType = m_TargetSceneChange;
		m_pCurrentScene->Init(this, m_pInput, m_pIO);
	}
}

void GameState::Draw() {
	assert(0 != m_pCurrentScene);
	
	// Draw scene
	m_pCurrentScene->Draw();

}

void GameState::ChangeWindowTitle(const char* title) {
	m_pCore->SetWindowTitle(title);
}

void GameState::ChangeScene(SCENE_TYPE type) {
	m_bOnSceneChange = true;
	m_TargetSceneChange = type;
}

void GameState::Exit() {
	m_pCore->StopGame();
}
