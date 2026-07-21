#include "ScenePKGSelect.hpp"
#include "cassert"
#include "Output.h"
#include "IO.h"
#include "OpenGL/Shader.h"
#include "GameState.hpp"
#include "DirectXMath.h"

ScenePKGSelect::ScenePKGSelect() {
	m_2DShader = -1;
	m_3DShader = -1;
	m_pState = nullptr;
	m_XIndex = 0;
	m_YIndex = 0;
	m_LeftOptIndex = 0;
	m_XOptionDelay = 0.0f;
	m_YOptionDelay = 0.0f;
	m_pInput = nullptr;
}

ScenePKGSelect::~ScenePKGSelect() {

}

bool ScenePKGSelect::Init(GameState* state, InputDevice* input) {
	assert(nullptr != state);
	LOG_INFO("Initializing Package Select Scene");

	m_pState = state;
	m_pInput = input;

	m_TexMan.Init();
	CreateShaders();
	InitializeBackground();

	m_XIndex = 0;
	m_YIndex = 0;

	m_LeftOptIndex = 0;
	m_XOptionDelay = 0.5f;
	m_YOptionDelay = 0.5f;

	LOG_INFO("Finished PKGSelect init");
	return true;
}

void ScenePKGSelect::Move(float dt) {
	const float delay = 0.3;
	
	// Handle horizontal options movement
	if(m_XOptionDelay <= 0.0f) {
		
		if(m_pInput->GetKeyPress(GLFW_KEY_RIGHT)/*Input up / right*/) {
			m_XIndex++;
			if(m_XIndex > 1) m_XIndex = 0;
			m_XOptionDelay = delay;
		} else if(m_pInput->GetKeyPress(GLFW_KEY_LEFT)/*Input down / left*/) {
			m_XIndex--;
			if(m_XIndex < 0) m_XIndex = 1;
			m_XOptionDelay = delay;
		}

		if(m_pInput->GetKeyPress(GLFW_KEY_Z)) {
			HandleOptions();
		}
	}
	else {
		m_XOptionDelay -= dt;
	}

	// Handle vertical options movement
	if(m_YOptionDelay <= 0.0f) {
		if(m_pInput->GetKeyPress(GLFW_KEY_UP)) {
			switch(m_XIndex) {
				case 0:
					m_LeftOptIndex++;
					if(m_LeftOptIndex > 1) m_LeftOptIndex = 0;
					m_YOptionDelay = delay;
					break;
				case 1:
					m_YIndex++;
					if(m_YIndex > 1) m_YIndex = 0;
					m_YOptionDelay = delay;
					break;
			}
		} else if(m_pInput->GetKeyPress(GLFW_KEY_DOWN)) {
			switch(m_XIndex) {
				case 0:
					m_LeftOptIndex--;
					if(m_LeftOptIndex < 0) m_LeftOptIndex = 1;
					m_YOptionDelay = delay;
					break;
				case 1:
					m_YIndex--;
					if(m_YIndex < 0) m_YIndex = 1;
					m_YOptionDelay = delay;
					break;
			}
		}
		
		if(m_pInput->GetKeyPress(GLFW_KEY_Z)) {
			HandleOptions();
		}
	}
	else {
		m_YOptionDelay -= dt;
	}

}

void ScenePKGSelect::Draw() {
	Enter2DMode();
	m_BGSprite.Draw();

	const uint32_t active_color = 0xffffffff;
	const uint32_t inactive_color = 0x80ffffff;
	
	// Display left options
	switch(m_LeftOptIndex) {
		case 0:
			DrawString(&m_Font, 64.0f, 320.0f, "Return to Main", active_color);
			DrawString(&m_Font, 64.0f, 360.0f, "Refresh", inactive_color);
			break;
		case 1:	
			DrawString(&m_Font, 64.0f, 320.0f, "Return to Main", inactive_color);
			DrawString(&m_Font, 64.0f, 360.0f, "Refresh", active_color);
			break;
	}
	
	char buf[256] = "";
	sprintf(buf, "LeftOpt: %d", m_LeftOptIndex);
	DrawString(&m_Font, 64.0f, 100.0f, buf, 0xff00ffff);
	sprintf(buf, "XIndex: %d", m_XIndex);
	DrawString(&m_Font, 64.0f, 140.0f, buf, 0xff00ffff);
	sprintf(buf, "YIndex: %d", m_YIndex);
	DrawString(&m_Font, 64.0f, 180.0f, buf, 0xff00ffff);

	// Display Packages and their names
	
}

void ScenePKGSelect::InitializeBackground() {
	LOG_INFO("Creating Background objects");
	char* data = nullptr;
	size_t size = 0;
	LoadDataFromFile("DAT/pkg_select.png", (void**)&data, &size);
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

void ScenePKGSelect::CreateShaders() {
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

void ScenePKGSelect::Enter2DMode() {
	glUseProgram(m_2DShader);	
	glDisable(GL_DEPTH_TEST);
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 640.0f, 480.0f, 0.0f, 1.0f, -1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&proj);
}

void ScenePKGSelect::Enter3DMode() {

}

void ScenePKGSelect::HandleOptions() {
	switch(m_XIndex) {
	case 0: // Left options (go back, refresh)
		switch(m_LeftOptIndex) {
		case 0: // Return to previous menu
			assert(nullptr != m_pState);
			m_pState->ChangeScene(SCENE_MAIN);
			break;
		case 1: // Refresh
			//m_pState->Exit();
			break;
		}
		break;
	case 1: // Packages
		
		break;
	}

}

