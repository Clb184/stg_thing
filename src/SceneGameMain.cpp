#include "SceneGameMain.hpp"
#include "Output.h"
#include "cassert"
#include "GameState.hpp"
#include "XASM2/VM.hpp"
#include "OpenGL/Shader.h"
#include "IO.h"
#include "InputDevice.hpp"
#include "Misc/Primitives.h"

SceneGameMain::SceneGameMain() {
	m_Score = 0;
	m_ScoreMax = 0;
	m_2DShader = -1;
	m_3DShader = -1;
	m_DebugKeyWait = 0.5f;
	m_pInput = nullptr;
	m_pState = nullptr;
}

SceneGameMain::~SceneGameMain() {

}

void SceneGameMain::SetResourceRoot(const char* resource_root) {
	std::cout << "Setting resource root for GameMain: " << resource_root << "\n";
	m_ResourceRoot = resource_root;
}

bool SceneGameMain::Init(GameState* state, InputDevice* input) {
	assert(nullptr != state);

	LOG_INFO("Initializing GameMain");
	
	m_pState = state;
	m_pInput = input;

	m_TexMan.Init();
	CreateShaders();
	CreateBackground();
	LoadPackResources();

	// Set XASM2 seed
	XASM2RandomInit(123);
	

	return true;
}

void SceneGameMain::Move(float dt) {

	if(m_DebugKeyWait > 0.0f) {
		m_DebugKeyWait -= dt;
	}

	if(m_pInput->GetKeyPress(GLFW_KEY_ESCAPE)) {
		m_pState->ChangeScene(SCENE_PKGSEL);
		return;
	}
	// Debug restart
	if(m_pInput->GetKeyPress(GLFW_KEY_R)) {
		Init(m_pState, m_pInput);
		m_DebugKeyWait = 1.0f;
	}
}

void SceneGameMain::Draw() {
	// Draw background
	Enter3DMode();
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBBG3D);
	glViewport(0, 0, 400, 480);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_VA3D);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 960);

	// Draw UI and others
	Enter2DMode();
	m_PPBG.Draw();
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
		LOG_ERROR("Failed creating 2D Shader");
	}
	m_2DShader = prog;

	LoadShaderFromFile("DAT/Transform3D.vert", &vs, GL_VERTEX_SHADER);
	LoadShaderFromFile("DAT/Transform3D.frag", &fs, GL_FRAGMENT_SHADER);
	if(false == CreateShaderProgram(vs, fs, &prog)){
		LOG_ERROR("Failed creating 3D Shader");
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
	CreateRenderTexture(&m_BG3D, &m_FBBG3D, 400, 480);
	m_PPBG.Init();
	m_PPBG.SetTexID(m_BG3D);
	m_PPBG.SetPos(16.0f + 200.0f, 240.0f);
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

void SceneGameMain::LoadPackResources() {
	if(0 == PackFileOpen(&m_Pack, m_ResourceRoot.c_str())) {
		LOG_INFO("Loading packed file...");
		char* data;
		size_t size;
		if(0 == PackFileLoadEntry(&m_Pack, "level.json", (void**)&data, &size)) {
			nlohmann::json jsn = nlohmann::json::parse(data);
			if(jsn.find("binary") != jsn.end()) {
				LOG_INFO("Loading binary file");
			} else {
				LOG_ERROR("File must have at least a binary entry");
				PackFileClose(&m_Pack);
			}
		}

	} else {
		LOG_ERROR("File provided is not a valid Archive, skipping");
	}
}

void SceneGameMain::Enter2DMode() {
	glUseProgram(m_2DShader);	
	glDisable(GL_DEPTH_TEST);
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 640.0f, 480.0f, 0.0f, 1.0f, -1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&proj);

}

void SceneGameMain::Enter3DMode() {
	glUseProgram(m_3DShader);
	glEnable(GL_DEPTH_TEST);
}
