#ifndef SCENEGAMEMAIN_INCLUDED
#define SCENEGAMEMAIN_INCLUDED

#include "Scene.hpp"

class GameState;

class SceneGameMain : public Scene {
public:
	SceneGameMain();
	~SceneGameMain();

	// Inherited
	bool Init(void* data);
	void Move(float dt);
	void Draw();

private:
	int m_NumStages;
	//STGStage* m_pStages;

	GameState* m_pState;
};

#endif
