#ifndef TASKCAMERA_INCLUDED
#define TASKCAMERA_INCLUDED

#include "DirectXMath.h"
#include "XASM2/VM.hpp"

class TaskCamera {
public:
	TaskCamera();
	~TaskCamera();

	bool Init();
	bool Setup(uint8_t* script);
	void Move(float dt)

	Camera& GetCamera();
	const DirectX::XMFLOAT3& GetPos() const;
	const DirectX::XMFLOAT3& GetRot() const;

private:
	// Camera class
	Camera m_Camera;

	// Member regs
	DirectX::XMFLOAT3 m_CamPos;
	DirectX::XMFLOAT3 m_CamRot;

	xasm2_vm_t m_Task;

};

#endif
