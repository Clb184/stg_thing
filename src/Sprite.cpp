#include "Sprite.hpp"
#include "Misc/Primitives.h"
#include "cstring"
#include "DirectXMath.h"

Sprite::Sprite() {
	m_bNeedUpdate = true;
	m_VB = -1;
	m_VA = -1;
	//memset(&m_SpriteInf, 0, sizeof(sprite_inf_t));
}

Sprite::~Sprite() {
	glDeleteBuffers(1, &m_VB);
	glDeleteBuffers(1, &m_VA);
}

bool Sprite::Init() {
	GLERR;
	CreateTL2DVertexBuffer(4, nullptr, GL_MAP_WRITE_BIT, &m_VB, &m_VA);
	GL_ERROR();
	return true;
}

void Sprite::SetTexID(GLuint id) {
	m_SpriteInf.tex_id = id;
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
	GLERR;
	if(true) {
		TLVertex2D* vertices = (TLVertex2D*)glMapNamedBuffer(m_VB, GL_WRITE_ONLY);
		assert(nullptr != vertices);
		float c = DirectX::XMScalarCos(m_SpriteInf.ang), s = DirectX::XMScalarSin(m_SpriteInf.ang),
		      hw = m_SpriteInf.w * 0.5f, hh = m_SpriteInf.h * 0.5f;
		vertices[0].x = (c * -hw - s * -hh) + m_SpriteInf.x; 
		vertices[0].y = (s * -hw + c * -hh) + m_SpriteInf.y; 
		vertices[0].u = m_SpriteInf.u0;
		vertices[0].v = m_SpriteInf.v0;
		vertices[0].color = m_SpriteInf.color;

		vertices[1].x = (c * -hw - s * hh) + m_SpriteInf.x; 
		vertices[1].y = (s * -hw + c * hh) + m_SpriteInf.y; 
		vertices[1].u = m_SpriteInf.u0;
		vertices[1].v = m_SpriteInf.v1;
		vertices[1].color = m_SpriteInf.color;

		vertices[2].x = (c * hw - s * -hh) + m_SpriteInf.x; 
		vertices[2].y = (s * hw + c * -hh) + m_SpriteInf.y;
		vertices[2].u = m_SpriteInf.u1;
		vertices[2].v = m_SpriteInf.v0;
		vertices[2].color = m_SpriteInf.color;

		vertices[3].x = (c * hw - s * hh) + m_SpriteInf.x; 
		vertices[3].y = (s * hw + c * hh) + m_SpriteInf.y; 
		vertices[3].u = m_SpriteInf.u1;
		vertices[3].v = m_SpriteInf.v1;
		vertices[3].color = m_SpriteInf.color;

		glUnmapNamedBuffer(m_VB);
		GL_ERROR();
		m_bNeedUpdate = false;
	}
	glBindTextureUnit(0, m_SpriteInf.tex_id);
	glBindVertexArray(m_VA);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		GL_ERROR();
}
