#ifndef STAGECTRL_INCLUDED
#define STAGECTRL_INCLUDED

#include "TaskStage.hpp"
#include "BackgroundCtrl.hpp"

class StageCtrl {
public:
	StageCtrl();
	~StageCtrl();

	bool Init(BackgroundCtrl* bg);
	bool SetupTask(uint8_t* base, int offset);
	void Move(float dt);
	void Draw();
	void Cleanup();
private:
	TaskStage m_Stage;
};

#endif
