#include "SpriteManager.hpp"
#include "cassert"
#include "Output.h"

SpriteManager::SpriteManager() {

}

SpriteManager::~SpriteManager() {

}

bool SpriteManager::Init(int cnt) {
	LOG_INFO("Initializing Sprite Manager");
	m_pSpriteData = (sprite_inf_t*)calloc(cnt, sizeof(sprite_inf_t));
	return true;
}

void SpriteManager::InitDrawList() {
	m_bEnabled = true;
	m_SpriteIndex = 0;
}

sprite_inf_t* SpriteManager::NextSprite() {
	assert(nullptr != m_pSpriteData);
	if(m_SpriteIndex >= m_MaxSprites || false == m_bEnabled) return nullptr;

	return &m_pSpriteData[m_SpriteIndex++];
}


