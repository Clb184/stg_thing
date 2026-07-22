#include "GameState.hpp"
#include "Output.h"
#include "cassert"
#include "GameCore.hpp"

GameState::GameState(GameCore* core) {
	m_pCurrentScene = nullptr;

	m_pMain = new SceneMain();
	m_pPKGSel = new ScenePKGSelect();
	m_pTitle = new SceneTitle();
	m_pGameMain = new SceneGameMain();
	
	m_pCore = core;

	m_bOnSceneChange = false;
	m_TargetSceneChange = SCENE_TITLE;
	m_CurrentSceneType = SCENE_NULL;
}

GameState::~GameState() {
	m_pCurrentScene = nullptr;
	if(nullptr != m_pMain) delete m_pMain;
	if(nullptr != m_pTitle) delete m_pTitle;
	if(nullptr != m_pGameMain) delete m_pGameMain;
}

bool GameState::Init(InputDevice* input) {
	LOG_INFO("Initializing GameState");
	char buf[512] = "";
	// Check if all scenes are initialized
	if(nullptr == m_pMain || nullptr == m_pTitle || nullptr == m_pGameMain)	{
		LOG_ERROR("Failed to allocate memory for scenes:");
		sprintf(buf, "Main: %p, Title: %p, GameMain: %p", m_pMain, m_pTitle, m_pGameMain);
		LOG_ERROR(buf);
		return false;
	}

	// Main Scene is the default scene
	m_pCurrentScene = m_pMain;
	m_CurrentSceneType = SCENE_MAIN;
	
	// For debug purpose
	//m_pCurrentScene = m_pGameMain;
	//m_CurrentSceneType = SCENE_GAMEMAIN;
	//m_pGameMain->SetResourceRoot("pkgs/test/");

	m_pInput = input;

	if(false == m_pCurrentScene->Init(this, input)) {
		LOG_ERROR("Failed initializing default scene");
		return false;
	}

	return true;
}

void GameState::Move(float dt) {
	assert(0 != m_pCurrentScene);

	m_pCurrentScene->Move(dt);

	if(m_bOnSceneChange) {
		switch(m_TargetSceneChange) {
			default: m_TargetSceneChange = SCENE_MAIN;
			case SCENE_MAIN: m_pCurrentScene = m_pMain;  break;
			case SCENE_PKGSEL: m_pCurrentScene = m_pPKGSel; break;
			case SCENE_TITLE: m_pCurrentScene = m_pTitle; break;
			case SCENE_GAMEMAIN: m_pCurrentScene = m_pGameMain; break;
		}

		m_bOnSceneChange = false;

		m_CurrentSceneType = m_TargetSceneChange;
		m_pCurrentScene->Init(this, m_pInput);
	}


}

void GameState::Draw() {
	assert(0 != m_pCurrentScene);

	m_pCurrentScene->Draw();

}

void GameState::ChangeWindowTitle(const char* title) {
	m_pCore->SetWindowTitle(title);
}

std::string GameState::GetFetchURL() {
	return m_pCore->GetConfigCtrl()->GetFetchURL();
}

void GameState::ChangeScene(SCENE_TYPE type) {
	m_bOnSceneChange = true;
	m_TargetSceneChange = type;
}

void GameState::PrepareGameMain(const char* resource_root) {
	m_pGameMain->SetResourceRoot(resource_root);
}

void GameState::Exit() {
	m_pCore->StopGame();
}
