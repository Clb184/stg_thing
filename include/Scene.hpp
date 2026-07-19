#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED

class GameState;

class Scene {
	public:
		virtual bool Init(GameState* state, void*) = 0;
		virtual void Move(float dt) = 0;
		virtual void Draw() = 0;
};

#endif
