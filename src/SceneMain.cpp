#include "SceneMain.hpp"
#include "cassert"
#include "Output.h"
#include "GameState.hpp"

SceneMain::SceneMain() {

}

SceneMain::~SceneMain() {

}

bool SceneMain::Init(GameState* state, void* data) {
	assert(nullptr != state);
	LOG_INFO("Initializing Main Scene");
	return true;
}

void SceneMain::Move(float dt) {

}

void SceneMain::Draw() {

}
