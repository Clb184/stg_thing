#ifndef SCENEGAMEMAIN_INCLUDED
#define SCENEGAMEMAIN_INCLUDED

#include "Scene.hpp"
#include "GL/glew.h"
#include "cstdint"

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

	void Enter2DMode();
	void Enter3DMode();
private:
	GLuint m_2DShader;
	GLuint m_3DShader;

private:
	int64_t m_Score;
	int64_t m_ScoreMax;

private:

	//STGStage* m_pStages;
	InputDevice* m_pInput;
	GameState* m_pState;
};

#endif
