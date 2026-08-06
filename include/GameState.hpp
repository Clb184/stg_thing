#ifndef GAMESTATE_INCLUDED
#define GAMESTATE_INCLUDED

#include "Scene.hpp"
#include "SceneMain.hpp"
#include "SceneTitle.hpp"
#include "SceneGameMain.hpp"


enum SCENE_TYPE : int {
	SCENE_MAIN,
	SCENE_TITLE,
	SCENE_GAMEMAIN,
	SCENE_NULL = -1
};

class GameCore;

class GameState {
public:
	GameState(GameCore* core);
	~GameState();

	// Inherited
	bool Init(InputDevice* input, ScreenOutput* IO);
	void Move(float dt);
	void Draw();

	void ChangeWindowTitle(const char* title);
	
	void ChangeScene(SCENE_TYPE type);
	void Exit();

private:
	bool m_bOnSceneChange;
	int m_TargetSceneChange;
	
	Scene* m_pCurrentScene;
	int m_CurrentSceneType;

	SceneMain* m_pMain;
	SceneGameMain* m_pGameMain;
	
	InputDevice* m_pInput;
	ScreenOutput* m_pIO;
	GameCore* m_pCore;
};

#endif
