#ifndef SCENETITLE_INCLUDED
#define SCENETITLE_INCLUDED

#include "Scene.hpp"

class SceneTitle : public Scene {
public:
	SceneTitle();
	~SceneTitle();

	// Inherited
	bool Init(GameState* state, InputDevice* data);
	void Move(float dt);
	void Draw();

private:

};

#endif
