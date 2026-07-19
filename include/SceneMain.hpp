#ifndef SCENEMAIN_INCLUDED
#define SCENEMAIN_INCLUDED

#include "Scene.hpp"

/*
 *	Main Title Scene
 *	Scene where you can actually start the game
 */
class SceneMain : public Scene {
public:
	SceneMain();
	~SceneMain();

	// Inherited
	bool Init(void* data);
	void Move(float dt);
	void Draw();
private:
};

#endif
