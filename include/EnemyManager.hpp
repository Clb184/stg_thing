#ifndef ENEMYMANAGER_INCLUDED
#define ENEMYMANAGER_INCLUDED

#include "SpriteManager.hpp"
#include "BackgroundCtrl.hpp"
#include "SparseSet.hpp"
#include "EntityManager.hpp"
#include "XASM2/VM.hpp"
#include "List.hpp"
#include "vector"

#include "DirectXMath.h"

#define ENEMY_MAX 3000

class EnemyManager {
public:
	struct ExtManagers {
		EnemyManager* enm;
		BackgroundCtrl* bg;
	};

public:
	EnemyManager();
	~EnemyManager();

	void Init(uint8_t* base, BackgroundCtrl* bgctrl);

	void Move(float dt);
	void Draw();

	void SetPos(int idx, float x, float y);
	DirectX::XMFLOAT2 GetPos(int idx);
	void AddEnemy(float x, float y, int hp, uint32_t offset);
	
	void SetCameraTask(uint32_t offset);
private:
	void ApplyDamage();

	void Delete(Entity id);
private:
	uint8_t* m_pBase;
	BackgroundCtrl* m_pBGCtrl;
	GLuint m_Blank;
	EntityManager<ENEMY_MAX> m_Entities;
	FixedSparseSet<ENEMY_MAX> m_Set;

	DirectX::XMFLOAT2 m_EnemyPos[ENEMY_MAX];
	int m_HP[ENEMY_MAX];
	int m_SpriteID[ENEMY_MAX];
	xasm2_vm_t m_VM[ENEMY_MAX];
	
	std::vector<Entity> m_FreeList;
	SpriteBatcher<ENEMY_MAX, 3> m_Batcher;
};

#endif
