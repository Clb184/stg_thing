#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED

class GameState;
class InputDevice;

class Scene {
	public:
		virtual bool Init(GameState* state, InputDevice* input) = 0;
		virtual void Move(float dt) = 0;
		virtual void Draw() = 0;
};

#endif
