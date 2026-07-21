#ifndef GAMESTATE_INCLUDED
#define GAMESTATE_INCLUDED

#include "Scene.hpp"
#include "SceneMain.hpp"
#include "ScenePKGSelect.hpp"
#include "SceneTitle.hpp"
#include "SceneGameMain.hpp"


enum SCENE_TYPE : int {
	SCENE_MAIN,
	SCENE_PKGSEL,
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
	bool Init(InputDevice* input);
	void Move(float dt);
	void Draw();

	void ChangeWindowTitle(const char* title);
	
	std::string GetFetchURL();
	void ChangeScene(SCENE_TYPE type);
	void Exit();

private:
	bool m_bOnSceneChange;
	int m_TargetSceneChange;
	
	Scene* m_pCurrentScene;
	int m_CurrentSceneType;

	SceneMain* m_pMain;
	ScenePKGSelect* m_pPKGSel;
	SceneTitle* m_pTitle;
	SceneGameMain* m_pGameMain;

	InputDevice* m_pInput;

	GameCore* m_pCore;
};

#endif
