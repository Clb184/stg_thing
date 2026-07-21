#include "SceneMain.hpp"
#include "cassert"
#include "Output.h"
#include "IO.h"
#include "GameState.hpp"
#include "nlohmann/json.hpp"
#include "OpenGL/Shader.h"
#include "DirectXMath.h"

SceneMain::SceneMain() {
	m_2DShader = -1;
	m_3DShader = -1;
	m_pState = nullptr;
	m_OptionIndex = 0;
	m_OptionDelay = 0.0f;
}

SceneMain::~SceneMain() {

}

bool SceneMain::Init(GameState* state, InputDevice* input) {
	assert(nullptr != state);
	LOG_INFO("Initializing Main Scene");
	m_pState = state;
	m_pInput = input;
	m_OptionDelay = 0.5f;

	m_TexMan.Init();
	LoadFromJSON("DAT/main.json");
	CreateShaders();
	CreateBackground();
	state->ChangeWindowTitle("Baretto | Build " __DATE__);
	return true;
}

void SceneMain::Move(float dt) {
	const float delay = 0.3;

	if(m_OptionDelay <= 0.0f) {
		
		if(m_pInput->GetKeyPress(GLFW_KEY_UP) || m_pInput->GetKeyPress(GLFW_KEY_RIGHT)/*Input up / right*/) {
			m_OptionIndex++;
			if(m_OptionIndex > 1) m_OptionIndex = 0;
			m_OptionDelay = delay;
		} else if(m_pInput->GetKeyPress(GLFW_KEY_DOWN) || m_pInput->GetKeyPress(GLFW_KEY_LEFT)/*Input down / left*/) {
			m_OptionIndex--;
			if(m_OptionIndex < 0) m_OptionIndex = 1;
			m_OptionDelay = delay;
		}

		if(m_pInput->GetKeyPress(GLFW_KEY_Z)) {
			switch(m_OptionIndex) {
				case 0: // Enter Package select
					m_pState->ChangeScene(SCENE_PKGSEL);
					break;
				case 1: // Exit
					m_pState->Exit();
					break;

			}
		}

	} else {
		m_OptionDelay -= dt;
	}
}

void SceneMain::Draw() {
	Enter3DMode();
	Enter2DMode();
	m_BGSprite.Draw();

	const uint32_t active_color = 0xffffffff;
	const uint32_t inactive_color = 0x80ffffff;
	

	switch(m_OptionIndex) {
		case 0: 
			DrawString(&m_Font, 64.0f, 320.0f, "Select Package", active_color);
			DrawString(&m_Font, 64.0f, 360.0f, "Exit", inactive_color);
			break;
		case 1:
			DrawString(&m_Font, 64.0f, 320.0f, "Select Package", inactive_color);
			DrawString(&m_Font, 64.0f, 360.0f, "Exit", active_color);
			break;
	}
}

bool SceneMain::LoadFromJSON(const char* source) {
	return true;
}

void SceneMain::CreateShaders() {
	LOG_INFO("Creating Shaders");
	GLuint vs, fs, prog;
	LoadShaderFromFile("DAT/T&L2D.vert", &vs, GL_VERTEX_SHADER);
	LoadShaderFromFile("DAT/T&L2D.frag", &fs, GL_FRAGMENT_SHADER);
	if(false == CreateShaderProgram(vs, fs, &prog)) {
		LOG_ERROR("Failed creating 2D Shader");
	}
	m_2DShader = prog;

	LoadShaderFromFile("DAT/T&L3D.vert", &vs, GL_VERTEX_SHADER);
	LoadShaderFromFile("DAT/T&L3D.frag", &fs, GL_FRAGMENT_SHADER);
	if(false == CreateShaderProgram(vs, fs, &prog)){
		LOG_ERROR("Failed creating 3D Shader");
	}
	m_3DShader = prog;
}

void SceneMain::CreateBackground() {
	LOG_INFO("Creating Background objects");
	char* data = nullptr;
	size_t size = 0;
	LoadDataFromFile("DAT/baretto_title.png", (void**)&data, &size);
	m_BGSprite.Init();
	m_BGSprite.SetTexID(m_TexMan.LoadTexture(data, size));
	m_BGSprite.SetPos(320.0f, 240.0f);
	m_BGSprite.SetSize(640.0f, 480.0f);
	m_BGSprite.SetUV(0.0f, 0.0f, 1.0f, 1.0f);
	free(data);
	
	InitializeFreeType(&m_FTLib);
	LoadFontFromFile(m_FTLib, &m_Desc, "DAT/PermanentMarker.ttf");
	

	CreateFontWithAtlas(m_Desc, &m_Font, 32);

}

void SceneMain::Enter2DMode() {
	glUseProgram(m_2DShader);	
	glDisable(GL_DEPTH_TEST);
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 640.0f, 480.0f, 0.0f, 1.0f, -1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&proj);
}

void SceneMain::Enter3DMode() {

}

