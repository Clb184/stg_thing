#ifndef TASKSTAGE_INCLUDED
#define TASKSTAGE_INCLUDED

#include "XASM2/VM.hpp"
#include "BackgroundCtrl.hpp"

struct TaskStage {
	TaskStage();
	~TaskStage();

	void Init();
	bool Setup(uint8_t* base, int offset);
	void Move(float dt);

	xasm2_vm_t task;
	int GRI[4];
	float GRF[4];
	BackgroundCtrl* bg_ctrl;
};

#endif
