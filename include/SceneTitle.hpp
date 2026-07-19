#ifndef SCENETITLE_INCLUDED
#define SCENETITLE_INCLUDED

#include "Scene.hpp"

class SceneTitle : public Scene {
public:
	SceneTitle();
	~SceneTitle();

	// Inherited
	bool Init(void* data);
	void Move(float dt);
	void Draw();

private:

};

#endif
