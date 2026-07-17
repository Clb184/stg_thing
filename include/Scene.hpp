#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED

class Scene {
	public:
		virtual bool Init(void*) = 0;
		virtual void Move(float dt) = 0;
		virtual void Draw();
};

#endif
