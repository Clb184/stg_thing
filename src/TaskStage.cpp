#include "TaskStage.hpp"
#include "GameCore.hpp"
#include "cstring"

TaskStage::TaskStage() {
	memset(GRI, 0x00, sizeof(int) * 4);
	memset(GRF, 0x00, sizeof(int) * 4);
}

TaskStage::~TaskStage() {

}

void TaskStage::Init() {
	XASM2VMZeroInit(&task);
	memset(GRI, 0x00, sizeof(int) * 4);
	memset(GRF, 0x00, sizeof(int) * 4);
}

bool TaskStage::Setup(uint8_t* base, int offset) {
	XASM2VMInit(&task, base, offset);
	XASM2VMSetMembers(&task, 8, GRI);
	return true;
}

int XASM2StageTask(uint8_t cmd, xasm2_vm_t* vm, float dt, void* data) {
	TaskStage* task = (TaskStage*)data;
	switch(cmd) {
		case 0x80: g_Sound.MusicLoad(vm->r1.i); break;
		case 0x81: g_Sound.MusicPlay(); break;
		case 0x82: g_Sound.MusicPause(); break;
		case 0x83: g_Sound.MusicStop(); break;
	}
	return 0;
}

void TaskStage::Move(float dt) {
	XASM2Move(&task, dt, XASM2StageTask, this);
}
