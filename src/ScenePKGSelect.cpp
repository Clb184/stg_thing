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
	m_YIndexMax = 0;
	m_LeftOptIndex = 0;

	m_FailLive = 0.0f;
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
	
	m_FailLive = 0.0f;

	m_PKGMan.Init();
	m_PKGMan.SetFetchURL(state->GetFetchURL());
	if(false == m_PKGMan.Refresh()) {
		m_FailLive = 10.0f;
	}
	m_TexMan.Init();
	CreateShaders();
	InitializeBackground();

	m_XIndex = 0;
	m_YIndex = 0;

	m_LeftOptIndex = 0;
	m_YIndexMax = m_PKGMan.GetPkgCount();

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
			if(m_PKGMan.GetAvailable(m_YIndex)) {
				if(m_XIndex > 2) m_XIndex = 2;
			} else {
				if(m_XIndex > 1) m_XIndex = 1;
			}
			m_XOptionDelay = delay;
		} else if(m_pInput->GetKeyPress(GLFW_KEY_LEFT)/*Input down / left*/) {
			m_XIndex--;
			/*if(m_PKGMan.GetAvailable(m_YIndex)) {
				if(m_XIndex < 0) m_XIndex = 0;
			} else {
			}*/
			if(m_XIndex < 0) m_XIndex = 0;
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
					m_YIndex--;
					if(m_YIndex < 0) m_YIndex = m_YIndexMax -1;
					m_YOptionDelay = 0.15f;
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
					m_YIndex++;
					if(m_YIndex >= m_YIndexMax) m_YIndex = 0;
					m_YOptionDelay = 0.15f;
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

	if(m_FailLive > 0.0f) {
		m_FailLive -= dt;
	}

}

void ScenePKGSelect::Draw() {
	Enter2DMode();
	m_BGSprite.Draw();

	const uint32_t active_alpha = 0xff000000;
	const uint32_t inactive_alpha = 0x80000000;
	
	// Display left options
	uint32_t alpha = inactive_alpha;
	if(m_XIndex == 0) alpha = active_alpha;
	switch(m_LeftOptIndex) {

		case 0:
			DrawString(&m_Font, 64.0f, 320.0f, "Return to Main", alpha | 0xddffff);
			DrawString(&m_Font, 64.0f, 360.0f, "Refresh", inactive_alpha | 0xddffff);
			break;
		case 1:	
			DrawString(&m_Font, 64.0f, 320.0f, "Return to Main", inactive_alpha | 0xddffff);
			DrawString(&m_Font, 64.0f, 360.0f, "Refresh", alpha | 0xddffff);
			break;
	}

	// Debug variables
	char buf[512] = "";
	sprintf(buf, "Packages: %d", m_PKGMan.GetPkgCount());
	DrawString(&m_SmallFont, 16.0f, 20.0f, buf, 0xff00ffff);
	sprintf(buf, "LeftOpt: %d", m_LeftOptIndex);
	DrawString(&m_SmallFont, 16.0f, 46.0f, buf, 0xff00ffff);
	sprintf(buf, "XIndex: %d", m_XIndex);
	DrawString(&m_SmallFont, 16.0f, 72.0f, buf, 0xff00ffff);
	sprintf(buf, "YIndex: %d", m_YIndex);
	DrawString(&m_SmallFont, 16.0f, 98.0f, buf, 0xff00ffff);
	sprintf(buf, "YIndexMax: %d", m_YIndexMax);
	DrawString(&m_SmallFont, 16.0f, 124.0f, buf, 0xff00ffff);

	// Display Packages and their names
	for(int i = 0; i < m_YIndexMax; i++) {
		alpha = inactive_alpha;
		if(i == m_YIndex && m_XIndex == 1) alpha = active_alpha;
		//sprintf(buf, "Name: %s", m_PKGMan.GetEntryName(i).c_str());
		//LOG_INFO(buf);
		DrawString(&m_SmallFont, 300.0f, 48.0f + i * 36.0f, m_PKGMan.GetEntryName(i).c_str(), alpha | 0x80ffff);
		std::string by_author = "by: " + m_PKGMan.GetEntryAuthor(i);
		DrawString(&m_SmallFont, 360.0f, 60.0f + i * 36.0f, by_author.c_str(), alpha | 0xddffff);
		if(m_PKGMan.GetAvailable(i)) {
			alpha = inactive_alpha;
			if(m_XIndex == 2 && i == m_YIndex) alpha = active_alpha;
			DrawString(&m_Font, 500.0f, 50.0f + i * 36.0f, "Play", alpha | 0x5080ff);
		}
	}
	if(m_FailLive > 0.0f) {
		DrawString(&m_SmallFont, 64.0f, 400.0f, "Failed to communicate with server, try again later", 0xff1010dd);
	}

	
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
	CreateFontWithAtlas(m_Desc, &m_SmallFont, 24);

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
			m_PKGMan.SaveCache();
			m_pState->ChangeScene(SCENE_MAIN);
			break;
		case 1: // Refresh
			if(false == m_PKGMan.Refresh()) {
				m_FailLive = 10.0f;
			}
			m_YIndexMax = m_PKGMan.GetPkgCount();
			if(m_YIndex >= m_YIndexMax) {
				m_YIndex = m_YIndexMax - 1;
			}
			break;
		}
		break;
	case 1: // Packages
		m_PKGMan.DownloadEntry(m_YIndex);
		break;
	case 2: // Package play
		m_pState->PrepareGameMain(m_PKGMan.GetPkgDir(m_YIndex).c_str());
		m_pState->ChangeScene(SCENE_GAMEMAIN);
		break;
	}
	m_XOptionDelay = 0.5f;
	m_YOptionDelay = 0.5f;
}

