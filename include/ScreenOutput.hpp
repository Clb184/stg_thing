#ifndef SCREENOUTPUT_INCLUDED
#define SCREENOUTPUT_INCLUDED

#include "OpenGL/Font.h"
#include "vector"
#include "string"

struct message_t {
	int type;
	uint32_t color;
	float time; // In seconds
	std::string msg;
};

class ScreenOutput {
private:
	const uint32_t INFO_COLOR = 0xffffffff;
	const uint32_t WARN_COLOR = 0xff22ffff;
	const uint32_t ERR_COLOR = 0xff2222ff;

	enum msg_type {
		MSG_INFO,
		MSG_WARN,
		MSG_ERR
	};

public:
	ScreenOutput();
	~ScreenOutput();

	bool Init();
	
	void LogInfo(const char * msg);
	void LogInfo(const std::string& msg);
	void LogWarning(const char* msg);
	void LogWarning(const std::string& msg);
	void LogError(const char* msg);
	void LogError(const std::string& msg);

	void Move(float dt);
	void Draw();

private:
	void CreateShaders();
	void Cleanup();

private:
	GLuint m_Program;
	float m_FPS;
	int m_Width;
	int m_Height;
	float m_XBegin;
	float m_YBegin;
	float m_VSpace;

	std::vector<message_t> m_MsgQueue;

	FT_Library m_FTLib;
	font_t m_Font;
	font_descriptor_t m_Desc;

};

#endif
