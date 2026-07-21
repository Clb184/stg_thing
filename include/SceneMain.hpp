#ifndef SCENEMAIN_INCLUDED
#define SCENEMAIN_INCLUDED

#include "Scene.hpp"
#include "Sprite.hpp"
#include "SpriteManager.hpp"
#include "TextureManager.hpp"
#include "OpenGL/Font.h"
#include "InputDevice.hpp"

/*
 *	Main Title Scene
 *	Scene where you can actually start the game
 */
class SceneMain : public Scene {
public:
	SceneMain();
	~SceneMain();

	// Inherited
	bool Init(GameState* state, InputDevice* data);
	void Move(float dt);
	void Draw();
private:
	bool LoadFromJSON(const char* source);
	void CreateShaders();
	void CreateBackground();

	void Enter2DMode();
	void Enter3DMode();

private:
	GLuint m_2DShader;
	GLuint m_3DShader;
	TextureManager m_TexMan;
	SpriteManager m_SpriteMan;
	Sprite m_BGSprite;
	GameState* m_pState;

private: // Text and related
	FT_Library m_FTLib;
	font_t m_Font;
	font_descriptor_t m_Desc;
	int m_OptionIndex;
	float m_OptionDelay;
	InputDevice* m_pInput;
};

#endif
