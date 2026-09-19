#include "EnemyManager.hpp"

EnemyManager::EnemyManager() {

}

EnemyManager::~EnemyManager() {

}

void EnemyManager::Init() {
	InitSingleList(&m_Enemies);
	m_Entities.Init();
	m_Set.Init();
	m_Batcher.Init();
}

int EnmCallback(uint8_t cmd, xasm2_vm_t* vm, float dt, void* data) {
	switch(cmd) {
		case 0x80:
			break;
		default:
			return -1;
	}
	return 0;
};

void EnemyManager::Move(float dt) {
	if(m_Enemies.count <= 0) return;
	for (node_t<Entity>* e = m_Enemies.head; e != 0; e = e->next) {
		uint32_t id = m_Set.Get(e->data);
		XASM2Move(&m_VM[id], dt, EnmCallback, this);
	}
}

void EnemyManager::Draw() {
	//for
}
