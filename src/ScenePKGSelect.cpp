#include "ScenePKGSelect.hpp"
#include "cassert"
#include "Output.h"
#include "GameState.hpp"

ScenePKGSelect::ScenePKGSelect() {

}

ScenePKGSelect::~ScenePKGSelect() {

}

bool ScenePKGSelect::Init(GameState* state, void* data) {
	assert(nullptr != state);
	LOG_INFO("Initializing Package Select Scene");
	return true;
}

void ScenePKGSelect::Move(float dt) {

}

void ScenePKGSelect::Draw() {

}
