#include "SceneGameMain.hpp"
#include "Output.h"
#include "cassert"
#include "GameState.hpp"
#include "XASM2/VM.hpp"
#include "OpenGL/Shader.h"
#include "IO.h"
#include "InputDevice.hpp"
#include "Misc/Primitives.h"
#include "iostream"
#include "nlohmann/json.hpp"
#include "GameWindow.hpp"

SceneGameMain::SceneGameMain() {
	m_Score = 0;
	m_ScoreMax = 0;

	m_2DShader = 0;
	m_3DShader = 0;

	m_DebugKeyWait = 0.5f;
	m_pInput = nullptr;
	m_pState = nullptr;

	m_Desc = nullptr;
	memset(&m_GameAreaTex, 0, sizeof(render_texture_t));
	memset(&m_Font, 0, sizeof(m_Font));
	memset(&m_FTLib, 0, sizeof(m_FTLib));
}

SceneGameMain::~SceneGameMain() {
	Cleanup();
}

auto ToRad = [](float deg) { return deg * 3.14159f / 180.0f; };

bool SceneGameMain::Init(GameState* state, InputDevice* input, ScreenOutput* IO) {
	assert(nullptr != state);

	LOG_INFO("Initializing GameMain");
	Cleanup();
	m_DebugKeyWait = 1.0f;

	m_pState = state;
	m_pInput = input;
	m_Out = IO;

	m_TexMan.Init(IO);
	CreateShaders();
	CreateBackground();
	m_BGCtrl.Init(&m_TexMan);
	m_BGCtrl.SetDebugControl(input);
	LoadFirstPackResources();
	
	// Set XASM2 seed
	XASM2RandomInit(123);
	m_Timer = 0.0f;


	return true;
}

void SceneGameMain::Move(float dt) {
	m_Timer += dt;
	if(m_DebugKeyWait > 0.0f) {
		m_DebugKeyWait -= dt;
	}

	if(m_pInput->GetKeyPress(GLFW_KEY_ESCAPE)) {
		MusicStop(&g_Sound);
		m_pState->ChangeScene(SCENE_MAIN);
		return;
	}

	// Debug restart
	if(m_pInput->GetKeyPress(GLFW_KEY_R)) {
		Init(m_pState, m_pInput, m_Out);
	}
	
	m_BGCtrl.Move(dt);
}

void SceneGameMain::Draw() {
	// Draw background
	Enter3DMode();


	// Setup viewport and 3D
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, 400, 480);
	
	// Setup camera and world light
	m_BGCtrl.Draw();


	// Draw UI and others
	Enter2DMode();
	glBindFramebuffer(GL_FRAMEBUFFER, m_GameAreaTex.framebuffer);
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 400.0f, 480.0f, 0.0f, 1.0f, -1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&proj);
	m_PPBG.SetTexID(m_BGCtrl.GetTexture());
	m_PPBG.SetPos(200.0f, 240.0f);
	m_PPBG.Draw();

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 640.0f, 480.0f, 0.0f, 1.0f, -1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&proj);

	// Game Area
	m_PPBG.SetTexID(m_GameAreaTex.texture);
	m_PPBG.SetPos(16.0f + 200.0f, 240.0f);
	m_PPBG.Draw();

	// UI
	m_LeftUI.Draw();
	m_RightUI.Draw();

	char buf[128];
	sprintf(buf, "Score: %010d", m_Score);
	DrawString(&m_Font, 640.0f - 200.0f, 64.0f, buf, 0xff44eeee);
	sprintf(buf, "Max  : %010d", m_ScoreMax);
	DrawString(&m_Font, 640.0f - 200.0f, 86.0f, buf, 0xff44eeee);

	DrawCameraProps();
}

void SceneGameMain::DrawCameraProps() {
	const float xp = 32.0f, yp = 300.0f;
	char buf[256];
	auto ToDeg = [](float radian) {  return radian * 180.0f / 3.14159f; };
	DirectX::XMFLOAT4 pos = m_BGCtrl.GetCameraTask().light.cam_pos;
	DirectX::XMFLOAT4 rot = m_BGCtrl.GetCameraTask().rot;
	DirectX::XMFLOAT2 fog = m_BGCtrl.GetCameraTask().fog;
	DirectX::XMFLOAT4 fcolor = m_BGCtrl.GetCameraTask().light.fog_color;
	DirectX::XMFLOAT4 wlr = m_BGCtrl.GetCameraTask().light.global_light;

	DrawString(&m_Font, xp, yp, "Camera:", 0xff44eeee);
	sprintf(buf, "pos : %5.3f, %5.3f, %5.3f", pos.x, pos.y, pos.z);
	DrawString(&m_Font, xp, yp + 20.0f, buf, 0xff44eeee);
	sprintf(buf, "rot : %5.3f, %5.3f, %5.3f", ToDeg(rot.x), ToDeg(rot.y), ToDeg(rot.z));
	DrawString(&m_Font, xp, yp + 40.0f, buf, 0xff44eeee);

	DrawString(&m_Font, xp, yp + 60.0f, "Fog:", 0xff44eeee);
	sprintf(buf, "near : %5.3f, far : %5.3f", fog.x, fog.y);
	DrawString(&m_Font, xp, yp + 80.0f, buf, 0xff44eeee);
	sprintf(buf, "color : %5.3f, %5.3f, %5.3f", fcolor.x, fcolor.y, fcolor.z);
	DrawString(&m_Font, xp, yp + 100.0f, buf, 0xff44eeee);

	DrawString(&m_Font, xp, yp + 120.0f, "Global Light:", 0xff44eeee);
	sprintf(buf, "Rotation : %5.3f, %5.3f, %5.3f", ToDeg(wlr.x), ToDeg(wlr.y), ToDeg(wlr.z));
	DrawString(&m_Font, xp, yp + 140.0f, buf, 0xff44eeee);
}

void SceneGameMain::CreateShaders() {
	LOG_INFO("Creating Shaders");
	GLuint vs, fs, prog;
	LoadShaderFromFile("DAT/T&L2D.vert", &vs, GL_VERTEX_SHADER);
	LoadShaderFromFile("DAT/T&L2D.frag", &fs, GL_FRAGMENT_SHADER);
	if(false == CreateShaderProgram(vs, fs, &prog)) {
		m_Out->LogError("Failed creating 2D Shader");
	}
	m_2DShader = prog;

	LoadShaderFromFile("DAT/T&L3D.vert", &vs, GL_VERTEX_SHADER);
	LoadShaderFromFile("DAT/T&L3D.frag", &fs, GL_FRAGMENT_SHADER);
	if(false == CreateShaderProgram(vs, fs, &prog)){
		m_Out->LogError("Failed creating 3D Shader");
	}
	m_3DShader = prog;
}

void SceneGameMain::CreateBackground() {


	GLuint tex = m_TexMan.Load("GRP/ui.png");
	texture_metric_t metric = m_TexMan.GetTextureMetrics(0);
	m_LeftUI.Init();
	m_LeftUI.SetTexID(tex);
	m_LeftUI.SetPos(8.0f, 240.0f);
	m_LeftUI.SetSize(16.0f, 480.0f);
	m_LeftUI.SetUV(0.0f, 0.0f, metric.texelw * 16.0f, metric.texelh * 480.0f);

	m_RightUI.Init();
	m_RightUI.SetTexID(tex);
	m_RightUI.SetPos(640.0f - 112.0f, 240.0f);
	m_RightUI.SetSize(224.0f, 480.0f);
	m_RightUI.SetUV(metric.texelw * 16.0f, 0.0f, metric.texelw * (16.0f + 224.0f), 1.0f);

	// 3D background
	CreateRenderTextureA(&m_GameAreaTex, 400, 480, 0);
	m_PPBG.Init();
	m_PPBG.SetSize(400.0f, 480.0f);
	m_PPBG.SetUV(0.0f, 1.0f, 1.0f, 0.0f);

	InitializeFreeType(&m_FTLib);
	LoadFontFromFile(m_FTLib, &m_Desc, "DAT/PermanentMarker.ttf");
	CreateFontWithAtlas(m_Desc, &m_Font, 20);
	
	// Load script data
	size_t size = 0;
	uint8_t* dat = 0;
	LoadDataFromFile("camera.dat", (void**)&dat, &size);
	m_BGCtrl.SetupTask(dat);
	data = (char*)dat;

}

bool SceneGameMain::LoadFirstPackResources() {
	char buf[512];
	pack_file_t pack;
	std::string level = "";
	std::string bgm = "";
	if(0 == PackFileOpen(&pack, "STG.DAT")) {
		LOG_INFO("Loading STG.DAT...");
		char* data;
		size_t size;
		// test level
		if(0 == PackFileLoadEntry(&pack, "game.json", (void**)&data, &size)) {
			nlohmann::json jsn = nlohmann::json::parse(data);
			if(jsn.find("test_level") != jsn.end()) {
				level = jsn["test_level"];
				m_Out->LogInfo("Loading demo level \"" + level + "\"");
			} else {
				m_Out->LogError("Demo level not found");
				return false;
			}

			if(jsn.find("bgm") != jsn.end()) {
				bgm = jsn["bgm"];
			} else {
				m_Out->LogError("BGM not found");
			}

			free(data);
		}
		else {
			m_Out->LogError("Failed to load level list");
			PackFileClose(&pack);
			return false;
		}

		// BGM
		if(0 == PackFileLoadEntry(&pack, bgm.c_str(), (void**)&data, &size)) {
			m_Out->LogInfo("Loading BGM \"" + bgm + "\"");
			if(0 != LoadMusicFromMemory(&g_Sound, (char*)data, size)) {
				m_Out->LogError("Couldn't load BGM");
			} else {
				//MusicEnableLoop(&g_Sound, 1);
				//MusicSetLoop(&g_Sound, 0, 0);
				MusicPlay(&g_Sound);
			}
			free(data);
		} else {
			m_Out->LogError("BGM not found");
		}

		PackFileClose(&pack);
	} else {
		m_Out->LogError("File provided is not a valid Archive, skipping");
	}
}

void SceneGameMain::LoadPackResources(const char* level) {

}

void SceneGameMain::Enter2DMode() {
	glUseProgram(m_2DShader);	
	glDisable(GL_DEPTH_TEST);

}

void SceneGameMain::Enter3DMode() {
	glUseProgram(m_3DShader);
	glEnable(GL_DEPTH_TEST);
}

void SceneGameMain::Cleanup() {
	glDeleteProgram(m_2DShader);
	glDeleteProgram(m_3DShader);

	m_2DShader = 0;
	m_3DShader = 0;

	DestroyRenderTexture(&m_GameAreaTex);
	DestroyFont(&m_Font);
	UninitializeFreeType(m_FTLib);
	m_TexMan.Cleanup();
}
