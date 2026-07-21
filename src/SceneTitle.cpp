#include "SceneTitle.hpp"
#include "cassert"
#include "Output.h"
#include "GameState.hpp"

SceneTitle::SceneTitle() {

}

SceneTitle::~SceneTitle() {

}

bool SceneTitle::Init(GameState* state, InputDevice* input) {
	assert(nullptr != state);
	LOG_INFO("Initializing Title Scene");
	return true;
}

void SceneTitle::Move(float dt) {

}

void SceneTitle::Draw() {

}
