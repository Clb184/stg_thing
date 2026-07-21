#include "SpriteManager.hpp"
#include "cassert"
#include "Output.h"
#include "DirectXMath.h"

SpriteManager::SpriteManager() {
	m_pSpriteData = nullptr;
	m_bEnabled  = false;
	m_MaxSprites = 0;
	m_SpriteIndex = 0;

	m_VB = -1;
	m_VA = -1;
	m_pTexMan = nullptr;
}

SpriteManager::~SpriteManager() {

}

bool SpriteManager::Init(int cnt, TextureManager* tex_man) {
	LOG_INFO("Initializing Sprite Manager");
	Cleanup();
	m_pSpriteData = (sprite_inf_t*)calloc(cnt, sizeof(sprite_inf_t));
	
	m_PBuffer.Init(sizeof(TLVertex2D) * cnt * 4, 3);

	attribute_info_t TL2DAttributes[] = {
		{0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 0)},
		{1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2)},
		{2, 4, GL_UNSIGNED_BYTE, GL_TRUE, (sizeof(float) * 4)}
	};

	buffer_info_t buffinfo = { m_PBuffer.GetBufferID(), sizeof(TLVertex2D)};

	CreateVertexAttribute(TL2DAttributes[0], buffinfo, &m_VA);
	CreateVertexAttribute(TL2DAttributes[1], buffinfo, &m_VA);
	CreateVertexAttribute(TL2DAttributes[2], buffinfo, &m_VA);

	//CreateTL2DVertexBuffer(cnt * 4 * 3 * sizeof(TLVertex2D), nullptr, GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT, &m_VB, &m_VA); // Triple buffering
	m_pTexMan = tex_man;
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

void SpriteManager::EndDrawList() {
	m_bEnabled = false;
}

void SpriteManager::Draw() {
	assert(nullptr != m_pSpriteData);
	assert(nullptr != m_pTexMan);
	if (m_SpriteIndex <= 0) return;
	
	TLVertex2D* vertices = (TLVertex2D*)m_PBuffer.RequestBuffer();

	for (int i = 0; i < m_SpriteIndex; i++) {
		const sprite_inf_t* inf = m_pSpriteData + i;
		float c = DirectX::XMScalarCos(inf->ang), s = DirectX::XMScalarSin(inf->ang),
		      hw = inf->w * 0.5f, hh = inf->h * 0.5f;
		vertices[i*4].x = (c * -hw - s * -hh) + inf->x; 
		vertices[i*4].y = (s * -hw + c * -hh) + inf->y; 
		vertices[i*4].u = inf->u0;
		vertices[i*4].v = inf->v0;
		vertices[i*4].color = inf->color;

		vertices[i*4+1].x = (c * -hw - s * hh) + inf->x; 
		vertices[i*4+1].y = (s * -hw + c * hh) + inf->y; 
		vertices[i*4+1].u = inf->u0;
		vertices[i*4+1].v = inf->v1;
		vertices[i*4+1].color = inf->color;

		vertices[i*4+2].x = (c * hw - s * -hh) + inf->x; 
		vertices[i*4+2].y = (s * hw + c * -hh) + inf->y;
		vertices[i*4+2].u = inf->u1;
		vertices[i*4+2].v = inf->v0;
		vertices[i*4+2].color = inf->color;

		vertices[i*4+3].x = (c * hw - s * hh) + inf->x; 
		vertices[i*4+3].y = (s * hw + c * hh) + inf->y; 
		vertices[i*4+3].u = inf->u1;
		vertices[i*4+3].v = inf->v1;
		vertices[i*4+3].color = inf->color;

	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VA);
	for(int i = 0; i < m_SpriteIndex; i++) {
		glBindTextureUnit(0, m_pSpriteData[i].tex_id);
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
	}
}

void SpriteManager::Cleanup() {
	if(nullptr != m_pSpriteData) {
		delete[] m_pSpriteData;
		m_pSpriteData = nullptr;
	}
	m_pTexMan = nullptr;
	glDeleteBuffers(1, &m_VA);
}
