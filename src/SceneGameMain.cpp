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

float x = 0.0f;
float y = -450.0f;
float z = 150.0f;

float pitch = 0.0f;
float yaw = 0.0f;
float roll = 0.0f;

float nearf = 400.0f;
float farf = 500.0f;
int colorf = 0xffaa0000;

struct CameraData {
	DirectX::XMMATRIX cam;
	DirectX::XMMATRIX vw;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4 fog = {100.0f, 150.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT4 _extra[2] = { {1.0f, 1.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f, 0.0f}};
} camera_data;

struct WorldLight {
	float global_light[4] = {0.0f, 0.5f, 1.5f, 1.0f};
	float ambient[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float fog_color[4] = {0.5f, 0.8f, 0.7f, 1.0f};
	float light_color[4] = {0.0f, 0.0f, 0.8f, 0.0f};
	float specular_power[4] = {0.5f, 0.0f, 0.0f, 0.0f};
	float cam_pos[4] = {x, y, z, 1.0f};
} world_light;

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
	InitializeCamera();
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
	if(m_pInput->GetKeyPress(GLFW_KEY_D)) {
		x += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_A)) {
		x -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_W)) {
		y += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_S)) {
		y -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_SPACE)) {
		z += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_RIGHT_CONTROL)) {
		z -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_Q)) {
		nearf += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_E)) {
		nearf -= 20.0f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_T)) {
		farf += 20.0f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_Y)) {
		farf -= 20.0f * dt;
	}
	

	if(m_pInput->GetKeyPress(GLFW_KEY_LEFT_SHIFT)) {
		yaw -= 0.314159f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_RIGHT_SHIFT)) {
		yaw += 0.314159f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_UP)) {
		roll += 0.314159f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_DOWN)) {
		roll -= 0.314159f * dt;
	}
	if(m_pInput->GetKeyPress(GLFW_KEY_LEFT)) {
		pitch -= 0.314159f * dt;
	}
	else if(m_pInput->GetKeyPress(GLFW_KEY_RIGHT)) {
		pitch += 0.314159f * dt;
	}
	// Debug restart
	if(m_pInput->GetKeyPress(GLFW_KEY_R)) {
		Init(m_pState, m_pInput, m_Out);
	}

}

void SceneGameMain::Draw() {
	// Draw background
	Enter3DMode();

	// Setup camera
	m_Camera.SetPos(x, y, z);
	m_Camera.SetRot(0.0f, 0.0f, -0.323);
	m_Camera.SetFog(nearf, farf);
	m_Camera.Update();
	m_Camera.SetBinding(0);

	// Update World light
	WorldLight* wl = (WorldLight*)glMapNamedBuffer(m_CBs[2], GL_WRITE_ONLY);
	world_light.global_light[0] = yaw;
	world_light.global_light[1] = pitch;
	world_light.global_light[2] = roll;
	world_light.cam_pos[0] = x;
	world_light.cam_pos[1] = y;
	world_light.cam_pos[2] = z;
	world_light.fog_color[0] = float(colorf & 0x000000ff) / 255.0f;
	world_light.fog_color[1] = float((colorf & 0x0000ff00) >> 8) / 255.0f;
	world_light.fog_color[2] = float((colorf & 0x00ff0000) >> 16) / 255.0f;
	memcpy(wl, &world_light, sizeof(WorldLight));
	glUnmapNamedBuffer(m_CBs[2]);
	BindConstantBuffer(m_CBs[2], 2);

	// Setup viewport and 3D
	glBindFramebuffer(GL_FRAMEBUFFER, m_3DBGTex.framebuffer);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, 400, 480);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Plane.GetTransform().SetRot(m_Timer * 0.2f, 3.14159f * 0.5f , 0.0f);
	m_Plane.GetTransform().Update();
	m_Plane.Draw();

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

	DrawCameraProps();
}

void SceneGameMain::DrawCameraProps() {
	const float xp = 100.0f, yp = 300.0f;
	char buf[256];
	auto ToDeg = [](float radian) {  return radian * 180.0f / 3.14159f; };
	DrawString(&m_Font, xp, yp, "Camera:", 0xff44eeee);
	sprintf(buf, "pos : %5.3f, %5.3f, %5.3f", x, y, z);
	DrawString(&m_Font, xp, yp + 20.0f, buf, 0xff44eeee);
	sprintf(buf, "rot : %5.3f, %5.3f, %5.3f", ToDeg(pitch), ToDeg(yaw), ToDeg(roll));
	DrawString(&m_Font, xp, yp + 40.0f, buf, 0xff44eeee);
	sprintf(buf, "near : %5.3f, far : %5.3f, color: %0000006X", nearf, farf, colorf);
	DrawString(&m_Font, xp, yp + 60.0f, buf, 0xff44eeee);

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
	
	m_Plane.Init(16, 16);
	const float grow = 16.0f;
	float mx = -64.0f, my = -64.0f;
	TLVertex3D* verts = new TLVertex3D[8 * 8 * 6];
	int mi = 0;
	for(int yi = 0; yi < 8; yi++) {
		mx = -64.0f;
		for(int xi = 0; xi < 8; xi++) {
			verts[mi * 6] = {mx + grow, my, 0.0f, 0xffffffff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 1] = {mx, my, 0.0f, 0xffffffff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 2] = {mx + grow, my + grow, 0.0f, 0xffffffff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 3] = {mx, my, 0.0f, 0xffffffff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 4] = {mx + grow, my + grow, 0.0f, 0xffffffff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
			verts[mi * 6 + 5] = {mx, my + grow, 0.0f, 0xffffffff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
			mx += grow;
			mi++;
		}
		my += grow;
	}
	/*TLVertex3D verts[4] = {
		{128.0f, -128.0f, 0.0f, 0xff0000ff, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
		{-128.0f, -128.0f, 0.0f, 0xff00ff00, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
		{128.0f, 128.0f, 0.0f, 0xffff0000, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
		{-128.0f, 128.0f, 0.0f, 0xffff00ff, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	};*/
	delete[] verts;
	
	glUseProgram(m_3DShader);
	// Camera
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(3.14159f * 0.25f, 480.0f / 400.0f, 0.1f, 1000.0f);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(_mm_set_ps(0.0, z, y, x), _mm_set_ps(0.0f, 140.0f, 0.0f, 0.0f), _mm_set_ps(0.0, 0.0, 1.0, 0.0));
	
	camera_data.cam = view * projection;
	camera_data.vw = view;
	camera_data.proj = projection;

	// Normal data
	struct {
		DirectX::XMMATRIX model_mat;
		DirectX::XMMATRIX normal_mat;
	} normals;
	normals.model_mat = DirectX::XMMatrixIdentity();
	//normals.model_mat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(DirectX::XMScalarSin(3.14159f * 0.25f * 0.5f), 0.0f, 0.0f, DirectX::XMScalarCos(3.14159f * 0.25f * 0.5f)));
	normals.normal_mat = DirectX::XMMatrixInverse(nullptr, normals.model_mat);



	buffer_descriptor_t buf_desc[3] = {
		{sizeof(camera_data), &camera_data, GL_DYNAMIC_DRAW},
		{sizeof(normals), &normals, GL_DYNAMIC_DRAW},
		{sizeof(world_light), &world_light, GL_DYNAMIC_DRAW}
	};
	CreateBuffers(buf_desc, m_CBs, 3);
	//BindConstantBuffer(m_CBs[0], 0);
	BindConstantBuffer(m_CBs[1], 1);
	BindConstantBuffer(m_CBs[2], 2);

}

void SceneGameMain::InitializeCamera() {
	glUseProgram(m_3DShader);
	m_Camera.Init();
	m_Camera.SetBinding(0);
	m_Camera.SetAspectRatio(400.0f, 480.0f);
	m_Camera.SetFOV(3.14159 * 0.25f);
	m_Camera.SetPos(x, y, z);
	m_Camera.SetRot(0.0f, 0.0f, 0.0f);
	m_Camera.Update();
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
				//MusicPlay(&g_Sound);
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
	glDeleteBuffers(3, m_CBs);

	m_2DShader = 0;
	m_3DShader = 0;

	m_CBs[0] = 0;
	m_CBs[1] = 0;
	m_CBs[2] = 0;
	
	DestroyRenderTexture(&m_3DBGTex);
	DestroyRenderTexture(&m_GameAreaTex);
	DestroyFont(&m_Font);
	UninitializeFreeType(m_FTLib);
	m_TexMan.Cleanup();
}
