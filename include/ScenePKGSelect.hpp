#ifndef SCENEPKSELECT_INCLUDED
#define SCENEPKSELECT_INCLUDED

#include "Scene.hpp"
#include "Sprite.hpp"
#include "OpenGL/Font.h"
#include "TextureManager.hpp"
#include "InputDevice.hpp"

class ScenePKGSelect : public Scene {
public:
	ScenePKGSelect();
	~ScenePKGSelect();

	bool Init(GameState* state, InputDevice* input);
	void Move(float dt);
	void Draw();

private:
	void InitializeBackground();
	void CreateShaders();

	void Enter2DMode();
	void Enter3DMode();

	void HandleOptions();

private:
	TextureManager m_TexMan;
	GLuint m_2DShader;
	GLuint m_3DShader;
	Sprite m_BGSprite;
	GameState* m_pState;

private:
	FT_Library m_FTLib;
	font_t m_Font;
	font_descriptor_t m_Desc;

	int m_XIndex;
	int m_YIndex;
	int m_LeftOptIndex;


	float m_XOptionDelay;
	float m_YOptionDelay;
	InputDevice* m_pInput;
	
};

#endif
