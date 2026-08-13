#ifndef BACKGROUNDCTRL_INCLUDED
#define BACKGROUNDCTRL_INCLUDED

#include "TaskCamera.hpp"

class BackgroundCtrl {
private:
	struct WorldLight {
		float global_light[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		float ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
		float fog_color[4] = {0.5f, 0.8f, 0.7f, 1.0f};
		float light_color[4] = {0.0f, 0.0f, 0.8f, 0.0f};
		float specular_power[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		float cam_pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	};
public:
	BackgroundCtrl();
	~BackgroundCtrl();

	void Init();
	void Move(float dt);
	void Draw();

	TaskCamera& GetCameraTask();
private:
	DirectX::XMFLOAT2 m_FogPos;
	
	TaskCamera m_Camera;
};

#endif
