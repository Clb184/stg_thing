#ifndef ENEMYMANAGER_INCLUDED
#define ENEMYMANAGER_INCLUDED

#include "SpriteManager.hpp"
#include "SparseSet.hpp"
#include "EntityManager.hpp"
#include "XASM2/VM.hpp"
#include "List.hpp"
#include "vector"

#include "DirectXMath.h"

#define ENEMY_MAX 3000

class EnemyManager;

struct enemy_t {
	Entity entity_id;
	EnemyManager* enm_man;
};

class EnemyManager {
public:

public:
	EnemyManager();
	~EnemyManager();

	void Init();

	void Move(float dt);
	void Draw();
private:
	list_t<Entity, ENEMY_MAX> m_Enemies;
	EntityManager<ENEMY_MAX> m_Entities;
	FixedSparseSet<ENEMY_MAX> m_Set;

	DirectX::XMFLOAT2 m_EnemyPos[ENEMY_MAX];
	int m_HP[ENEMY_MAX];
	int m_SpriteID[ENEMY_MAX];
	xasm2_vm_t m_VM[ENEMY_MAX];

	SpriteBatcher<ENEMY_MAX, 3> m_Batcher;
};

#endif
