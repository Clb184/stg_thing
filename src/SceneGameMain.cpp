#include "SceneGameMain.hpp"
#include "Output.h"
#include "cassert"
#include "GameState.hpp"
#include "XASM2/VM.hpp"

SceneGameMain::SceneGameMain() {

}

SceneGameMain::~SceneGameMain() {

}

bool SceneGameMain::Init(GameState* state, InputDevice* input) {
	assert(nullptr != state);

	LOG_INFO("Initializing GameMain");
	
	m_pState = state;

	// Set XASM2 seed
	XASM2RandomInit(123);


	return true;
}

void SceneGameMain::Move(float dt) {

}

void SceneGameMain::Draw() {


}
