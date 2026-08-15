#ifndef SCENEGAMEMAIN_INCLUDED
#define SCENEGAMEMAIN_INCLUDED

#include "Scene.hpp"
#include "GL/glew.h"
#include "OpenGL/Font.h"
#include "TextureManager.hpp"
#include "Sprite.hpp"
#include "cstdint"
#include "string"
#include "DirectXMath.h"
#include "ScreenOutput.hpp"
#include "Camera.hpp"
#include "3DPlane.hpp"
#include "BackgroundCtrl.hpp"

#include "PackArchive/PackFile.h"

class GameState;

class SceneGameMain : public Scene {
public:
	SceneGameMain();
	~SceneGameMain();

	// Inherited
	bool Init(GameState* state, InputDevice* input, ScreenOutput* IO);
	void Move(float dt);
	void Draw();

private:
		// Load Shaders
	void CreateShaders();
	void CreateBackground();
	void InitializeCamera();

	// Load packed resources
	bool LoadFirstPackResources();
	void LoadPackResources(const char* script);

	// Jump between draw modes
	void Enter2DMode();
	void Enter3DMode();

	void Cleanup();

private: // Debug
	void DrawCameraProps();

private: // Relevant shaders and sprites
	GLuint m_2DShader;
	GLuint m_3DShader;

	TextureManager m_TexMan;
	Sprite m_LeftUI;
	Sprite m_RightUI;
	Sprite m_PPBG; // Post Processed BackGround

private: // Score
	int64_t m_Score;
	int64_t m_ScoreMax;
	float m_Timer;

private: // Text
	
	FT_Library m_FTLib;
	font_t m_Font;
	font_descriptor_t m_Desc;

	ScreenOutput* m_Out;
	float m_DebugKeyWait;
	InputDevice* m_pInput;
	GameState* m_pState;

private: // Plane and related
	DirectX::XMMATRIX m_CameraMatrix;
	BackgroundCtrl m_BGCtrl;
	char* data;

	render_texture_t m_GameAreaTex;

	std::string m_ResourceRoot;

};

#endif
