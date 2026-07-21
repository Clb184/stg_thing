#include "SceneGameMain.hpp"
#include "Output.h"
#include "cassert"
#include "GameState.hpp"
#include "XASM2/VM.hpp"
#include "InputDevice.hpp"

SceneGameMain::SceneGameMain() {

}

SceneGameMain::~SceneGameMain() {

}

void SceneGameMain::SetResourceRoot(const char* resource_root) {
	std::cout << "Setting resource root for GameMain: " << resource_root << "\n";
}

bool SceneGameMain::Init(GameState* state, InputDevice* input) {
	assert(nullptr != state);

	LOG_INFO("Initializing GameMain");
	
	m_pState = state;
	m_pInput = input;
	// Set XASM2 seed
	XASM2RandomInit(123);
	

	return true;
}

void SceneGameMain::Move(float dt) {
	if(m_pInput->GetKeyPress(GLFW_KEY_ESCAPE)) {
		m_pState->ChangeScene(SCENE_PKGSEL);
		return;
	}
	// Debug restart
	if(m_pInput->GetKeyPress(GLFW_KEY_R)) {
		Init(m_pState, m_pInput);
	}
}

void SceneGameMain::Draw() {


}

void SceneGameMain::CreateShaders() {

}

void SceneGameMain::CreateBackground() {

}

void SceneGameMain::Enter2DMode() {

}

void SceneGameMain::Enter3DMode() {

}
