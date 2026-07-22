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

#include "PackArchive/PackFile.h"

class GameState;

class SceneGameMain : public Scene {
public:
	SceneGameMain();
	~SceneGameMain();

	// Inherited
	void SetResourceRoot(const char* resource_root);
	bool Init(GameState* state, InputDevice* input);
	void Move(float dt);
	void Draw();

private:
	void CreateShaders();
	void CreateBackground();
	void LoadPackResources();

	void Enter2DMode();
	void Enter3DMode();
private: // Relevant shaders and sprites
	GLuint m_2DShader;
	GLuint m_3DShader;

	TextureManager m_TexMan;
	Sprite m_LeftUI;
	Sprite m_RightUI;
	Sprite m_PPBG;

private: // Score
	int64_t m_Score;
	int64_t m_ScoreMax;

private: // Text
	
	FT_Library m_FTLib;
	font_t m_Font;
	font_descriptor_t m_Desc;

	float m_DebugKeyWait;
	//STGStage* m_pStages;
	InputDevice* m_pInput;
	GameState* m_pState;

private: // Plane and related
	DirectX::XMMATRIX m_CameraMatrix;
	GLuint m_Plane;
	GLuint m_VA3D;
	GLuint m_BG3D;
	GLuint m_FBBG3D;

	GLuint m_CBs[3];
	
	std::string m_ResourceRoot;
	pack_file_t m_Pack;
};

#endif
