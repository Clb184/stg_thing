#include "EnemyManager.hpp"
#include "GameCore.hpp"
#include "OpenGL/Texture.h"
#include "cassert"

EnemyManager::EnemyManager() {
	m_FreeList.reserve(ENEMY_MAX);
}

EnemyManager::~EnemyManager() {

}

void EnemyManager::Init(BackgroundCtrl* bgctrl) {
	m_Entities.Init();
	m_Set.Init();
	m_Batcher.Init();
	m_pBase = 0;
	m_FreeList.clear();
	m_pBGCtrl = bgctrl;
	CreateEmptyTexture(&m_Blank, 0xffffffff);
}

void EnemyManager::SetBaseAddress(uint8_t* base) {
	m_pBase = base;
}

int EnmCallback(uint8_t cmd, xasm2_vm_t* vm, float dt, void* data) {
	EnemyManager* enm = (EnemyManager*)data;
	switch(cmd) {
		case 0x80: // Get pos
		{
			DirectX::XMFLOAT2 pos = enm->GetPos(vm->extra_id);
			vm->r[0].f = pos.x;
			vm->r[1].f = pos.y;
		}
			break;
		case 0x81: // Set pos
			enm->SetPos(vm->extra_id, vm->r[0].f, vm->r[1].f);
			break;
		case 0x82: // ANM
			vm->cmd += 4;
			break;
		case 0x83: g_Sound.MusicLoad(vm->r[0].i); break;
		case 0x84: g_Sound.MusicPlay(); break;
		case 0x85: g_Sound.MusicPause(); break;
		case 0x86: g_Sound.MusicStop(); break;
		case 0x87: 
			enm->SetCameraTask(*(int*)vm->cmd);
			vm->cmd += 4;
			break;
		default:
			return -1;
	}
	return 0;
};

void EnemyManager::Move(float dt) {
	Entity* ents = m_Set.GetData();
	for (int i = 0; i < m_Set.Size(); i++) {
		uint32_t idx = m_Set.Get(ents[i]);
		m_VM[idx].extra_id = idx;
		XASM2Move(&m_VM[idx], dt, EnmCallback, this);
	}
}

void EnemyManager::Draw() {
	//for
}

void EnemyManager::SetPos(int idx, float x, float y) {
	m_EnemyPos[idx] = {x, y};
}

DirectX::XMFLOAT2 EnemyManager::GetPos(int idx) {
	return m_EnemyPos[idx];
}

void EnemyManager::AddEnemy(float x, float y, int hp, uint32_t offset) {
	Entity id = m_Entities.CreateEntity();
	int idx = m_Set.Add(id);
	m_EnemyPos[idx] = {x, y};
	m_HP[idx] = hp;

	XASM2VMInit(&m_VM[idx], m_pBase, offset);
}

void EnemyManager::SetCameraTask(uint32_t offset) {
	assert(0 != m_pBGCtrl);
	m_pBGCtrl->SetupTask(m_pBase, offset);
}

void EnemyManager::Delete(Entity id) {
	int thisi = m_Set.Get(id);
	m_Entities.DeleteEntity(id);
	m_Set.Delete(id);
	int last = m_Set.Size();

	// Swap and pop
	m_EnemyPos[thisi] = m_EnemyPos[last];
	m_HP[thisi] = m_HP[last];
	m_SpriteID[thisi] = m_SpriteID[last];
	m_VM[thisi] = m_VM[last];
}
