#include "StageCtrl.hpp"

StageCtrl::StageCtrl() {

}

StageCtrl::~StageCtrl() {

}

bool StageCtrl::Init(BackgroundCtrl* bg) {
	m_Stage.Init();
	m_Stage.bg_ctrl = bg;
	return true;
}

bool StageCtrl::SetupTask(uint8_t* base, int offset) {
	m_Stage.Setup(base, offset);
	return true;
}

void StageCtrl::Move(float dt) {
	m_Stage.Move(dt);
}

void StageCtrl::Draw() {

}

void StageCtrl::Cleanup() {

}
