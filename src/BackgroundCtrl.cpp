#include "BackgroundCtrl.hpp"

BackgroundCtrl::BackgroundCtrl() {

}

BackgroundCtrl::~BackgroundCtrl() {

}

void BackgroundCtrl::SetDebugControl(InputDevice* input) {
	m_pInput = input;
}

void BackgroundCtrl::Init() {
	m_Camera.Init();
}

void BackgroundCtrl::SetupTask(uint8_t* script) {
	m_Camera.Setup(script);
}

void BackgroundCtrl::Move(float dt) {
	MoveInput(dt);
	m_Camera.Move(dt);
}

void BackgroundCtrl::Draw() {
	m_Camera.Use();
}

TaskCamera& BackgroundCtrl::GetCameraTask() {
	return m_Camera;
}

void BackgroundCtrl::MoveInput(float dt) {
	if(0 != m_pInput) {
	if(m_pInput->GetKeyPress(GLFW_KEY_D)) {
		m_Camera.light.cam_pos.x += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_A)) {
		m_Camera.light.cam_pos.x -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_W)) {
		m_Camera.light.cam_pos.y += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_S)) {
		m_Camera.light.cam_pos.y -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_SPACE)) {
		m_Camera.light.cam_pos.z += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_LEFT_CONTROL)) {
		m_Camera.light.cam_pos.z -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_Q)) {
		m_Camera.fog.x += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_E)) {
		m_Camera.fog.x -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_T)) {
		m_Camera.fog.y += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_Y)) {
		m_Camera.fog.y -= 20.0f * dt;
	}
	

	if(m_pInput->GetKeyPress(GLFW_KEY_LEFT_SHIFT)) {
		m_Camera.rot.y -= 0.314159f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_RIGHT_SHIFT)) {
		m_Camera.rot.y += 0.314159f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_UP)) {
		m_Camera.rot.z += 0.314159f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_DOWN)) {
		m_Camera.rot.z -= 0.314159f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_LEFT)) {
		m_Camera.rot.x -= 0.314159f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_RIGHT)) {
		m_Camera.rot.x += 0.314159f * dt;
	}

	}
}
