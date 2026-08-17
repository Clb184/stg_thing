#include "BackgroundCtrl.hpp"

BackgroundCtrl::BackgroundCtrl() {

}

BackgroundCtrl::~BackgroundCtrl() {
	if(1 == CheckRenderTexture(&m_PPBG)) {
		DestroyRenderTexture(&m_PPBG);
	}
}

void BackgroundCtrl::SetDebugControl(InputDevice* input) {
	m_pInput = input;
}

void BackgroundCtrl::Init(TextureManager* texman) {
	m_Camera.Init();
	if(1 != CheckRenderTexture(&m_PPBG)) {
		CreateRenderTextureA(&m_PPBG, 400, 480, RTFLAG_DEPTH);
	}

	m_pTexMan = texman;

	m_Plane.Init(16, 16);
	m_Plane.SetTexture(m_pTexMan->Load("GRP/grass.png"));
}

GLuint BackgroundCtrl::GetTexture() const {
	return m_PPBG.texture;
}

void BackgroundCtrl::SetupTask(uint8_t* base, int offset) {
	m_Camera.Setup(base, offset);
}

void BackgroundCtrl::Move(float dt) {
	MoveInput(dt);
	m_Camera.Move(dt);
}

void BackgroundCtrl::Draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_PPBG.framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Camera.Use();

	//m_Plane.GetTransform().SetRot(m_Timer * 0.0f, 0.0 * 3.14159f * 0.5f , m_Timer * 0.2f);
	for(int i = 0; i < 3; i++) {
		m_Plane.GetTransform().SetPos(-256.0f, i * 256.0f, 0.0f);
		m_Plane.GetTransform().Update();
		m_Plane.Draw();
		m_Plane.GetTransform().SetPos(0.0f, i * 256.0f, 0.0f);
		m_Plane.GetTransform().Update();
		m_Plane.Draw();
		m_Plane.GetTransform().SetPos(256.0f, i * 256.0f, 0.0f);
		m_Plane.GetTransform().Update();
		m_Plane.Draw();
	}
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
