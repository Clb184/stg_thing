#include "ScreenOutput.hpp"
#include "DirectXMath.h"
#include "Output.h"
#include "OpenGL/Shader.h"

ScreenOutput::ScreenOutput() {
	m_Program = -1;
	m_Width = 0;
	m_Height = 0;
	m_MsgQueue = {};

	m_FTLib = nullptr;	
}

ScreenOutput::~ScreenOutput() {
	Cleanup();
}

bool ScreenOutput::Init() {
	LOG_INFO("Initializing Screen Output");
	Cleanup();
	m_Width = 640;
	m_Height = 480;
	m_XBegin = 0.0f;
	m_YBegin = 0.0f;
	m_VSpace = 16.0f;

	CreateShaders();
	InitializeFreeType(&m_FTLib);
	LoadFontFromFile(m_FTLib, &m_Desc, "DAT/PermanentMarker.ttf");
	CreateFontWithAtlas(m_Desc, &m_Font, 16.0f);

	m_MsgQueue.clear();

	return true;
}

const float timeout_msg = 5.0f; // 5 seconds

void ScreenOutput::LogInfo(const char* msg) {
	LOG_INFO(msg);
	m_MsgQueue.emplace_back(MSG_INFO, INFO_COLOR, timeout_msg, msg);
}

void ScreenOutput::LogInfo(const std::string& msg) {
	LOG_INFO(msg.c_str());
	m_MsgQueue.emplace_back(MSG_INFO, INFO_COLOR, timeout_msg, msg);
}

void ScreenOutput::LogWarning(const char* msg) {
	LOG_INFO(msg);
	m_MsgQueue.emplace_back(MSG_WARN, WARN_COLOR, timeout_msg, msg);
}

void ScreenOutput::LogWarning(const std::string& msg) {
	LOG_INFO(msg.c_str());
	m_MsgQueue.emplace_back(MSG_WARN, WARN_COLOR, timeout_msg, msg);
}

void ScreenOutput::LogError(const char* msg) {
	LOG_ERROR(msg);
	m_MsgQueue.emplace_back(MSG_ERR, ERR_COLOR, timeout_msg, msg);
}

void ScreenOutput::LogError(const std::string& msg) {
	LOG_ERROR(msg.c_str());
	m_MsgQueue.emplace_back(MSG_ERR, ERR_COLOR, timeout_msg, msg);
}

void ScreenOutput::Move(float dt) {
	for(int i = 0; i < m_MsgQueue.size(); i++) {
		if(m_MsgQueue[i].time <= 0.0) {
			m_MsgQueue.erase(m_MsgQueue.begin() + i);
			i--;
		} 
		else {
			m_MsgQueue[i].time -= dt;
		}
	}
}

void ScreenOutput::Draw() {
	glUseProgram(m_Program);
	glDisable(GL_DEPTH_TEST);
	float begin = m_YBegin;
	for(auto& msg : m_MsgQueue) {
		DrawString(&m_Font, m_XBegin, begin, msg.msg.c_str(), msg.color);
		begin += m_VSpace;
	}
}

void ScreenOutput::CreateShaders() {
	LOG_INFO("Creating Shaders");
	GLuint vs, fs, prog;
	LoadShaderFromFile("DAT/T&L2D.vert", &vs, GL_VERTEX_SHADER);
	LoadShaderFromFile("DAT/T&L2D.frag", &fs, GL_FRAGMENT_SHADER);
	if(false == CreateShaderProgram(vs, fs, &prog)) {
		LOG_ERROR("Failed creating 2D Shader");
	}
	m_Program = prog;

	glUseProgram(prog);
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 640.0f, 480.0f, 0.0f, 1.0f, -1.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&proj);
}

void ScreenOutput::Cleanup() {
	m_Width = 0;
	m_Height = 0;
	//glDeleteProgram(m_Program);

	UninitializeFreeType(m_FTLib);
	DestroyFont(&m_Font);
	m_MsgQueue.clear();
}
