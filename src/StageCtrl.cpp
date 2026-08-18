#include "StageCtrl.hpp"

StageCtrl::StageCtrl() {

}

StageCtrl::~StageCtrl() {

}

bool StageCtrl::Init() {
	m_Stage.Init();
	return true;
}

bool StageCtrl::SetupTask(uint8_t* base, int offset) {
	return m_Stage.Setup(base, offset);
}

void StageCtrl::Move(float dt) {
	m_Stage.Move(dt);
}

void StageCtrl::Draw() {

}

void StageCtrl::Cleanup() {

}
