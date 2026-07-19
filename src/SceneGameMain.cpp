#include "SceneGameMain.hpp"
#include "Output.h"
#include "cassert"
#include "GameState.hpp"
#include "XASM2/VM.hpp"

SceneGameMain::SceneGameMain() {

}

SceneGameMain::~SceneGameMain() {

}

bool SceneGameMain::Init(void* data) {
	assert(nullptr != data);

	LOG_INFO("Initializing GameMain");
	
	m_pState = (GameState*)data;

	// Set XASM2 seed
	XASM2RandomInit(123);


	return true;
}

void SceneGameMain::Move(float dt) {

}

void SceneGameMain::Draw() {


}
