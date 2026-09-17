#ifndef SPRITEMANAGER_INCLUDED
#define SPRITEMANAGER_INCLUDED

#include "cassert"
#include "string.h"
#include "Sprite.hpp"
#include "Misc/Primitives.h"
#include "PersistentBuffer.hpp"
#include "Output.h"

#include "DirectXMath.h"

template<int cnt>
class SpriteBatcher {
	template<const int cnt>
	struct SpriteInst {
		DirectX::XMFLOAT2 pos[cnt]; // Position X Y
		//DirectX::XMFLOAT2 cossin[cnt]; // Precalc cosine and sine
		float dir[cnt]; // Angle radians
		DirectX::XMFLOAT2 size[cnt]; // Size Width Height
		DirectX::XMFLOAT2 scale[cnt]; // Scale X Y
		uint32_t color[cnt];
		DirectX::XMFLOAT4 uv[cnt];
		GLuint tex_id[cnt];
	};
public:
	SpriteBatcher() {
		m_bEnabled  = false;
		m_SpriteIndex = 0;
		m_VA = 0;
		memset(&m_Inst, 0, sizeof(m_Inst));
	}

	~SpriteBatcher() {
		Cleanup();
	}

	bool Init() { // Max amount of accepted sprites
		LOG_INFO("Initializing Sprite Manager");
		GLERR;
		Cleanup();
	
		// 4 Vertices data, put into 123 234 indices sets for drawing batched quads 
		m_PBuffer.Init(sizeof(TLVertex2D) * cnt * 4, 3);
		GL_ERROR();
		attribute_info_t TL2DAttributes[] = {
			{0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 0)},
			{1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2)},
			{2, 4, GL_UNSIGNED_BYTE, GL_TRUE, (sizeof(float) * 4)}
		};
	
		buffer_info_t buffinfo = { m_PBuffer.GetBufferID(), sizeof(TLVertex2D)};
		glCreateVertexArrays(1, &m_VA);
		GL_ERROR();
		CreateVertexAttribute(TL2DAttributes[0], buffinfo, &m_VA);
		CreateVertexAttribute(TL2DAttributes[1], buffinfo, &m_VA);
		CreateVertexAttribute(TL2DAttributes[2], buffinfo, &m_VA);
		return true;
		
	}

	SpriteInst<cnt>& GetInstances() {
		return m_Inst;
	}

	void InitDrawList() { // Begin a draw list, current sprites = 0, enables the draw list
		m_bEnabled = true;
		m_SpriteIndex = 0;
	}
	
	int NextSprite() { // Gives a handle until reaching max
		if(m_SpriteIndex >= cnt || false == m_bEnabled)  {
			m_bEnabled = false;
			return -1;
		}
	
		return m_SpriteIndex++;
	}

	void EndDrawList() { // Ends the draw list
		m_bEnabled = false;
	}

	void Draw() { // Draw each sprite
		if (m_SpriteIndex <= 0) return;
		
		TLVertex2D* vertices = (TLVertex2D*)m_PBuffer.RequestBuffer();
	
		for (int i = 0; i < m_SpriteIndex; i++) {
			float c = DirectX::XMScalarCos(m_Inst.dir[i]), s = DirectX::XMScalarSin(m_Inst.dir[i]),
			      hw = m_Inst.size[i].x * 0.5f, hh = m_Inst.size[i].y * 0.5f;
			vertices[i*4].x = (c * -hw - s * -hh) + m_Inst.pos[i].x; 
			vertices[i*4].y = (s * -hw + c * -hh) + m_Inst.pos[i].y; 
			vertices[i*4].u = m_Inst.uv[i].x;
			vertices[i*4].v = m_Inst.uv[i].y;
			vertices[i*4].color = m_Inst.color[i];
	
			vertices[i*4+1].x = (c * -hw - s * hh) + m_Inst.pos[i].x;
			vertices[i*4+1].y = (s * -hw + c * hh) + m_Inst.pos[i].y;
			vertices[i*4+1].u = m_Inst.uv[i].x;
			vertices[i*4+1].v = m_Inst.uv[i].w;
			vertices[i*4+1].color = m_Inst.color[i];
	
			vertices[i*4+2].x = (c * hw - s * -hh) + m_Inst.pos[i].x;
			vertices[i*4+2].y = (s * hw + c * -hh) + m_Inst.pos[i].y;
			vertices[i*4+2].u = m_Inst.uv[i].z;
			vertices[i*4+2].v = m_Inst.uv[i].y;
			vertices[i*4+2].color = m_Inst.color[i];
	
			vertices[i*4+3].x = (c * hw - s * hh) + m_Inst.pos[i].x;
			vertices[i*4+3].y = (s * hw + c * hh) + m_Inst.pos[i].y;
			vertices[i*4+3].u = m_Inst.uv[i].z;
			vertices[i*4+3].v = m_Inst.uv[i].w;
			vertices[i*4+3].color = m_Inst.color[i];
	
		}
		
		glBindVertexArray(m_VA);
		GLuint current_tex = 0;
		for(int i = 0; i < m_SpriteIndex; i++) {
			if(current_tex != m_Inst.tex_id[i]) {
				current_tex = m_Inst.tex_id[i];
				glBindTextureUnit(0, current_tex);
			}
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		}
	}

	void Cleanup() {
		glDeleteVertexArrays(1, &m_VA);
		m_VA = 0;
	}

private:

private:
	bool m_bEnabled;
	GLuint m_VA; // Vertex Array
	GLuint m_IB; // Index Buffer
	int m_MaxSprites;
	int m_SpriteIndex;
	SpriteInst<cnt> m_Inst;
	PersistentBuffer m_PBuffer;
};

#endif
