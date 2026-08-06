#include "SceneMain.hpp"
#include "cassert"
#include "Output.h"
#include "IO.h"
#include "GameState.hpp"
#include "nlohmann/json.hpp"
#include "OpenGL/Shader.h"
#include "DirectXMath.h"

SceneMain::SceneMain() {
	m_2DShader = 0;
	m_3DShader = 0;

	m_pState = nullptr;
	m_OptionIndex = 0;
	m_OptionDelay = 0.0f;
	
	m_Desc = nullptr;
	memset(&m_Font, 0, sizeof(m_Font));
	memset(&m_FTLib, 0, sizeof(m_FTLib));
}

SceneMain::~SceneMain() {
	Cleanup();
}

bool SceneMain::Init(GameState* state, InputDevice* input, ScreenOutput* IO) {
	assert(nullptr != state);
	LOG_INFO("Initializing Main Scene");
	Cleanup();
	m_pState = state;
	m_pInput = input;
	m_OptionIndex = 0;
	m_WeaponSelectIndex = 0;
	m_OptionDelay = 1.0f;

	m_TexMan.Init();
	CreateShaders();
	CreateBackground();
	m_Out = IO;

	return true;
}

void SceneMain::Move(float dt) {
	const float delay = 0.3;
	
	if(m_OptionDelay <= 0.0f) {
		// Move over menu
		if(m_OptionIndex == 1) {
			if(m_pInput->GetKeyPress(GLFW_KEY_UP) || m_pInput->GetKeyPress(GLFW_KEY_LEFT)) {
				m_WeaponSelectIndex--;
				if(m_WeaponSelectIndex < 0) m_WeaponSelectIndex = 2;
				m_OptionDelay = delay;
			} else if(m_pInput->GetKeyPress(GLFW_KEY_DOWN) || m_pInput->GetKeyPress(GLFW_KEY_RIGHT)) {
				m_WeaponSelectIndex++;
				if(m_WeaponSelectIndex > 2) m_WeaponSelectIndex = 0;
				m_OptionDelay = delay;
			}
		}
		
		// Press Z to start game
		if(m_pInput->GetOK()) {
			switch(m_OptionIndex) {
			case 0:
				m_OptionIndex = 1;
				break;
			case 1:
				m_pState->ChangeScene(SCENE_GAMEMAIN);
				break;
			}
			m_OptionDelay = delay;
		}
		// Press Escape to exit
		else if(m_pInput->GetEscape()) {
			switch(m_OptionIndex) {
			case 0:
				m_pState->Exit();
				break;
			case 1:
				m_OptionIndex = 0;
				break;
			}
			m_OptionDelay = delay;
		}
		// Debuggggg
		if(m_pInput->GetKeyPress(GLFW_KEY_M)) {
			m_Out->LogInfo("This is information");
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
		DrawString(&m_Font, 240.0f, 360.0f, "Press Z to start", active_color);
		break;
	case 1:
		uint32_t colors[3];
		for(int i= 0; i < 3; i++) {
			colors[i] = inactive_color;
			if(i == m_WeaponSelectIndex)
				colors[i] = active_color;
		}

		DrawString(&m_Font, 40.0f, 200.0f, "Misaka & Kuroko", colors[0] );
		DrawString(&m_Font, 240.0f, 280.0f, "Misaka & Saten", colors[1] );
		DrawString(&m_Font, 440.0f, 200.0f, "Misaka & Uiharu", colors[2] );
		break;
	}
}

void SceneMain::Cleanup() {
	glDeleteProgram(m_2DShader);
	m_2DShader = 0;
	glDeleteProgram(m_3DShader);
	m_3DShader = 0;
	
	DestroyFont(&m_Font);
	UninitializeFreeType(m_FTLib);
	m_TexMan.Cleanup();
	m_SpriteMan.Cleanup();
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
	LoadDataFromFile("DAT/title.png", (void**)&data, &size);
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

