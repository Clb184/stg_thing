#include "Sprite.hpp"
#include "Misc/Primitives.h"
#include "cstring"

Sprite::Sprite() {
	m_bNeedUpdate = true;
	m_VB = -1;
	m_VA = -1;
	memset(&m_SpriteInf, 0, sizeof(sprite_inf_t));
}

Sprite::~Sprite() {
	glDeleteBuffers(1, &m_VB);
	glDeleteBuffers(1, &m_VA);
}

void Sprite::SetPos(float x, float y) {
	m_bNeedUpdate = true;
	m_SpriteInf.x = x;
	m_SpriteInf.y = y;
}

void Sprite::SetAngle(float dir) {
	m_bNeedUpdate = true;
	m_SpriteInf.ang = dir;
}

void Sprite::SetSize(float w, float h) {
	m_bNeedUpdate = true;
	m_SpriteInf.w = w;
	m_SpriteInf.h = h;
}

void Sprite::SetScale(float sx, float sy) {
	m_bNeedUpdate = true;
	m_SpriteInf.sx = sx;
	m_SpriteInf.sy = sy;
}

void Sprite::SetUV(float u0, float v0, float u1, float v1) {
	m_bNeedUpdate = true;
	m_SpriteInf.u0 = u0;
	m_SpriteInf.v0 = v0;
	m_SpriteInf.u1 = u1;
	m_SpriteInf.v1 = v1;
}

void Sprite::SetColor(uint32_t color) {
	m_SpriteInf.color = color;
}

void Sprite::Draw() {
	if(m_bNeedUpdate) {
		m_bNeedUpdate = false;
	}
}
