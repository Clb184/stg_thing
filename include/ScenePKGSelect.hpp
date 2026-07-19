#ifndef SCENEPKSELECT_INCLUDED
#define SCENEPKSELECT_INCLUDED

#include "Scene.hpp"

class ScenePKGSelect : public Scene {
public:
	ScenePKGSelect();
	~ScenePKGSelect();

	bool Init(GameState* state, void* data);
	void Move(float dt);
	void Draw();
};

#endif
