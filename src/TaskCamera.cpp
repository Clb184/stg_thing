#include "TaskCamera.hpp"

TaskCamera::TaskCamera() {
	m_CamPos = {0.0f, 0.0f, 0.0f};
	m_CamRot = {0.0f, 0.0f, 0.0f};
}

TaskCamera::~TaskCamera() {

}

bool TaskCamera::Init() {
	m_Camera.Init();
}

bool TaskCamera::Setup(uint8_t* script) {
	XASM2VMInit(m_Task, script);
	m_Task.member_reg = &m_CamPos;
}

void TaskCamera::Move(float dt) {
	XASM2Move(&m_Task, dt, nullptr);
	m_Camera.SetPos(m_CamPos.x, m_CamPos.y, m_CamPos.z);
	m_Camera.SetRot(m_CamRot.x, m_CamRot.y, m_CamRot.z);
	m_Camera.Update();
}

Camera& TaskCamera::GetCamera() {
	return m_Camera;
}

const DirectX::XMFLOAT3& TaskCamera::GetPos() const {
	return m_CamPos;
}

const DirectX::XMFLOAT3& TaskCamera::GetRot() const {
	return m_CamRot;
}
