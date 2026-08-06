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
	m_Plane = 0;
	m_VA3D = 0;
	m_CBs[0] = 0;
	m_CBs[1] = 0;
	m_CBs[2] = 0;

	m_DebugKeyWait = 0.5f;
	m_pInput = nullptr;
	m_pState = nullptr;

	m_Desc = nullptr;
	memset(&m_3DBGTex, 0, sizeof(render_texture_t));
	memset(&m_GameAreaTex, 0, sizeof(render_texture_t));
	memset(&m_Font, 0, sizeof(m_Font));
	memset(&m_FTLib, 0, sizeof(m_FTLib));
}

SceneGameMain::~SceneGameMain() {
	Cleanup();
}

bool SceneGameMain::Init(GameState* state, InputDevice* input, ScreenOutput* IO) {
	assert(nullptr != state);

	LOG_INFO("Initializing GameMain");
	Cleanup();
	m_DebugKeyWait = 1.0f;

	m_pState = state;
	m_pInput = input;
	m_Out = IO;

	m_TexMan.Init();
	CreateShaders();
	CreateBackground();
	LoadFirstPackResources();
	
	// Set XASM2 seed
	XASM2RandomInit(123);
	

	return true;
}

void SceneGameMain::Move(float dt) {

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

}

void SceneGameMain::Draw() {
	// Draw background
	Enter3DMode();
	glBindFramebuffer(GL_FRAMEBUFFER, m_3DBGTex.framebuffer);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, 400, 480);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_VA3D);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	// Draw UI and others
	Enter2DMode();
	glBindFramebuffer(GL_FRAMEBUFFER, m_GameAreaTex.framebuffer);
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 400.0f, 480.0f, 0.0f, 1.0f, -1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&proj);
	m_PPBG.SetTexID(m_3DBGTex.texture);
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

	LoadShaderFromFile("DAT/Transform3D.vert", &vs, GL_VERTEX_SHADER);
	LoadShaderFromFile("DAT/Transform3D.frag", &fs, GL_FRAGMENT_SHADER);
	if(false == CreateShaderProgram(vs, fs, &prog)){
		m_Out->LogError("Failed creating 3D Shader");
	}
	m_3DShader = prog;
}

void SceneGameMain::CreateBackground() {
	char* data;
	size_t size;

	LoadDataFromFile("DAT/ui.png", (void**)&data, &size);

	GLuint tex = m_TexMan.LoadTexture(data, size);
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
	CreateRenderTextureA(&m_3DBGTex, 400, 480, RTFLAG_DEPTH);
	CreateRenderTextureA(&m_GameAreaTex, 400, 480, 0);
	m_PPBG.Init();
	m_PPBG.SetSize(400.0f, 480.0f);
	m_PPBG.SetUV(0.0f, 1.0f, 1.0f, 0.0f);
	

	InitializeFreeType(&m_FTLib);
	LoadFontFromFile(m_FTLib, &m_Desc, "DAT/PermanentMarker.ttf");
	CreateFontWithAtlas(m_Desc, &m_Font, 20);
	
	TLVertex3D verts[4] = {
		{128.0f, -128.0f, 0.0f, 0xff0000ff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
		{-128.0f, -128.0f, 0.0f, 0xff00ff00, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
		{128.0f, 128.0f, 0.0f, 0xffff0000, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
		{-128.0f, 128.0f, 0.0f, 0xffff00ff, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	};
	CreateTL3DVertexBuffer(4, verts, GL_MAP_WRITE_BIT, &m_Plane, &m_VA3D);

	// Camera
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(3.14159f * 0.25f, 480.0f / 400.0f, 0.1f, 1000.0f);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(_mm_set_ps(0.0, 300.0, -300.0f, 0.0f), _mm_set_ps(0.0f, 140.0f, 0.0f, 0.0f), _mm_set_ps(0.0, 0.0, 1.0, 0.0));
	
	struct CameraData {
		DirectX::XMMATRIX cam;
		DirectX::XMMATRIX vw;
		DirectX::XMMATRIX proj;
		DirectX::XMFLOAT4 fog = {100.0f, 150.0f, 0.0f, 0.0f};
		DirectX::XMFLOAT4 _extra[2];
	} camera_data;
	camera_data.cam = view * projection;
	camera_data.vw = view;
	camera_data.proj = projection;

	// Normal data
	struct {
		float Model[16] = {
		DirectX::XMScalarCos(3.14159f * 0.25f),-DirectX::XMScalarSin(3.14159f * 0.25f), 0.0, 0.0,
		DirectX::XMScalarSin(3.14159f * 0.25f), DirectX::XMScalarCos(3.14159f * 0.25f), 0.0, 0.0,
		0.0, 0.0, 1.0f, 0.0,
		0.0, 0.0, 0.0, 1.0f,
		};
		float Normals[9] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
		};
	} normals;

	struct {
		float global_light[4] = {0.0f, 0.25f, 0.1f, 1.0f};
		float ambient[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float fog_color[4] = {0.5f, 0.8f, 0.7f, 1.0f};
		float light_color[4] = {1.0f, 0.8f, 0.8f, 0.0f};
		float specular_power[4] = {0.5f, 0.0f, 0.0f, 0.0f};
		float cam_pos[4] = {50.0f, 50.0f, 50.0f, 0.0f};
	} world_light;


	buffer_descriptor_t buf_desc[3] = {
		{sizeof(camera_data.cam), &camera_data.cam, GL_DYNAMIC_DRAW},
		{sizeof(normals.Model), normals.Model, GL_DYNAMIC_DRAW},
		{sizeof(world_light.ambient), world_light.ambient, GL_DYNAMIC_DRAW}
	};
	CreateBuffers(buf_desc, m_CBs, 3);
	BindConstantBuffer(m_CBs[0], 0);
	BindConstantBuffer(m_CBs[1], 1);
	BindConstantBuffer(m_CBs[2], 2);

}

bool SceneGameMain::LoadFirstPackResources() {
	char buf[512];
	pack_file_t pack;
	if(0 == PackFileOpen(&pack, "STG.DAT")) {
		LOG_INFO("Loading STG.DAT...");
		char* data;
		size_t size;
		// test level
		if(0 == PackFileLoadEntry(&pack, "game.json", (void**)&data, &size)) {
			nlohmann::json jsn = nlohmann::json::parse(data);
			if(jsn.find("test_level") != jsn.end()) {
				std::string level = jsn["test_level"];
				m_Out->LogInfo("Loading demo level \"" + level + "\"");
			} else {
				m_Out->LogError("Demo level not found");
				return false;
			}
			free(data);
		}
		else {
			m_Out->LogError("Failed to load level list");
			PackFileClose(&pack);
			return false;
		}

		// BGM
		if(0 == PackFileLoadEntry(&pack, "BGM/smml_demo_01.ogg", (void**)&data, &size)) {
			m_Out->LogInfo("Loading BGM");
			if(0 != LoadMusicFromMemory(&g_Sound, (char*)data, size)) {
				m_Out->LogError("Couldn't load BGM");
			} else {
				MusicSetLoop(&g_Sound, 852960, 852960 + 2116128);
				MusicEnableLoop(&g_Sound, 1);
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
	glDeleteBuffers(1, &m_Plane);
	glDeleteVertexArrays(1, &m_VA3D);
	glDeleteBuffers(3, m_CBs);

	m_2DShader = 0;
	m_3DShader = 0;
	m_Plane = 0;
	m_VA3D = 0;

	m_CBs[0] = 0;
	m_CBs[1] = 0;
	m_CBs[2] = 0;
	
	DestroyRenderTexture(&m_3DBGTex);
	DestroyRenderTexture(&m_GameAreaTex);
	DestroyFont(&m_Font);
	UninitializeFreeType(m_FTLib);
	m_TexMan.Cleanup();
}
