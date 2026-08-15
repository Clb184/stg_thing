#ifndef BACKGROUNDCTRL_INCLUDED
#define BACKGROUNDCTRL_INCLUDED

#include "TaskCamera.hpp"
#include "InputDevice.hpp"
#include "OpenGL/Texture.h"

class BackgroundCtrl {
private:
public:
	BackgroundCtrl();
	~BackgroundCtrl();
	
	void SetDebugControl(InputDevice* input);

	void Init();
	void SetupTask(uint8_t* script);
	void Move(float dt);
	void Draw();

	TaskCamera& GetCameraTask();
private:
	void MoveInput(float dt);
private:
	TaskCamera m_Camera;
	render_texture_t m_PPBG;

	InputDevice* m_pInput;
};

#endif
