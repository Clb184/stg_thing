#ifndef ENEMYMANAGER_INCLUDED
#define ENEMYMANAGER_INCLUDED

#include "SpriteManager.hpp"
#include "DirectXMath.h"

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Init();

	void Move(float dt);
	void Draw();
private:
	SpriteBatch<3000> m_Batcher;
};

#endif
