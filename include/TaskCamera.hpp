#ifndef TASKCAMERA_INCLUDED
#define TASKCAMERA_INCLUDED

#include "DirectXMath.h"
#include "XASM2/VM.hpp"
#include "Camera.hpp"

struct WorldLight {
	DirectX::XMFLOAT4 global_light = {0.0f, 0.0f, -0.78539f, 0.0f};
	DirectX::XMFLOAT4 ambient = { 0.04f, 0.04f, 0.04f, 0.0f };
	DirectX::XMFLOAT4 fog_color = {0.0f, 0.0f, 0.7f, 1.0f};
	DirectX::XMFLOAT4 light_color = {0.0f, 0.0f, 0.8f, 0.0f};
	float specular_power[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT4 cam_pos = {0.0f, 0.0f, 0.0f, 1.0f};
};

struct TaskCamera {
	TaskCamera();
	~TaskCamera();

	bool Init();
	void Use();
	bool Setup(uint8_t* script);
	void Move(float dt);
	
	Camera camera;

	// Member regs
	WorldLight light;
	DirectX::XMFLOAT4 rot;
	DirectX::XMFLOAT2 fog;
	float fov;

	GLuint wlight;
	xasm2_vm_t task;

};

#endif
