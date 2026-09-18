#ifndef SPRITEMANAGER_INCLUDED
#define SPRITEMANAGER_INCLUDED

#include "cassert"
#include "string.h"
#include "Sprite.hpp"
#include "Misc/Primitives.h"
#include "PersistentBuffer.hpp"
#include "Output.h"

#include "DirectXMath.h"

template<int cnt, int buffers>
class SpriteBatcher {
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
		m_IB = 0;
		m_pInst = nullptr;
	}

	~SpriteBatcher() {
		Cleanup();
	}

	bool Init() { // Initialize everything
		LOG_INFO("Initializing Sprite Manager");
		GLERR;
		Cleanup();
		
		// Indexes
		GLuint* indices = (GLuint*)malloc(sizeof(GLuint) * 6 * cnt);
		for(int i = 0; i < cnt; i++) {
			indices[i * 6] = i * 4;
			indices[i * 6 + 1] = i * 4 + 1;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 1;
			indices[i * 6 + 4] = i * 4 + 2;
			indices[i * 6 + 5] = i * 4 + 3;
		}

		buffer_descriptor_t ibuf_desc = { sizeof(GLuint) * 6 * cnt, indices, 0 }; // Don't need flags since is just uploading to GPU
		CreateStaticBuffer(ibuf_desc, &m_IB);
		free(indices);
		GL_ERROR();
	
		// 4 Vertices data, put into 123 234 indices sets for drawing batched quads 
		m_PBuffer.Init(sizeof(TLVertex2D) * cnt * 4);
		attribute_info_t TL2DAttributes[] = {
			{0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 0)},
			{1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2)},
			{2, 4, GL_UNSIGNED_BYTE, GL_TRUE, (sizeof(float) * 4)}
		};
		
		m_pInst = (SpriteInst*)calloc(sizeof(SpriteInst), 1);

		// Vertex array, linked with indices
		buffer_info_t buffinfo = { m_PBuffer.GetBufferID(), sizeof(TLVertex2D)};
		glCreateVertexArrays(1, &m_VA);
		GL_ERROR();
		CreateVertexAttribute(TL2DAttributes[0], buffinfo, &m_VA);
		CreateVertexAttribute(TL2DAttributes[1], buffinfo, &m_VA);
		CreateVertexAttribute(TL2DAttributes[2], buffinfo, &m_VA);
		glVertexArrayElementBuffer(m_VA, m_IB);

		return true;
		
	}

	SpriteInst* GetInstances() {
		return m_pInst;
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
		assert(m_pInst != 0);
		if (m_SpriteIndex <= 0) return;
		
		TLVertex2D* vertices = (TLVertex2D*)m_PBuffer.RequestBuffer();
	
		for (int i = 0; i < m_SpriteIndex; i++) {
			float c = DirectX::XMScalarCos(m_pInst->dir[i]), s = DirectX::XMScalarSin(m_pInst->dir[i]),
			      hw = m_pInst->size[i].x * 0.5f, hh = m_pInst->size[i].y * 0.5f;
			vertices[i*4].x = (c * -hw - s * -hh) + m_pInst->pos[i].x; 
			vertices[i*4].y = (s * -hw + c * -hh) + m_pInst->pos[i].y; 
			vertices[i*4].u = m_pInst->uv[i].x;
			vertices[i*4].v = m_pInst->uv[i].y;
			vertices[i*4].color = m_pInst->color[i];
	
			vertices[i*4+1].x = (c * -hw - s * hh) + m_pInst->pos[i].x;
			vertices[i*4+1].y = (s * -hw + c * hh) + m_pInst->pos[i].y;
			vertices[i*4+1].u = m_pInst->uv[i].x;
			vertices[i*4+1].v = m_pInst->uv[i].w;
			vertices[i*4+1].color = m_pInst->color[i];
	
			vertices[i*4+2].x = (c * hw - s * -hh) + m_pInst->pos[i].x;
			vertices[i*4+2].y = (s * hw + c * -hh) + m_pInst->pos[i].y;
			vertices[i*4+2].u = m_pInst->uv[i].z;
			vertices[i*4+2].v = m_pInst->uv[i].y;
			vertices[i*4+2].color = m_pInst->color[i];
	
			vertices[i*4+3].x = (c * hw - s * hh) + m_pInst->pos[i].x;
			vertices[i*4+3].y = (s * hw + c * hh) + m_pInst->pos[i].y;
			vertices[i*4+3].u = m_pInst->uv[i].z;
			vertices[i*4+3].v = m_pInst->uv[i].w;
			vertices[i*4+3].color = m_pInst->color[i];
	
		}
		
		glBindVertexArray(m_VA);
		GLuint current_tex = 0;
		int base = 0;
		int inst_cnt = 0;
		for(int i = 0; i < m_SpriteIndex; i++) {
			if(current_tex != m_pInst->tex_id[i]) {
				current_tex = m_pInst->tex_id[i];
				glBindTextureUnit(0, current_tex);
				if(0 != inst_cnt)
					glDrawElements(GL_TRIANGLES, base * 6, inst_cnt * 6, 0);
				base = i;
				inst_cnt = 0;
			}
			inst_cnt++;
		}
		if(inst_cnt > 0) {
			glDrawElements(GL_TRIANGLES, base * 6, inst_cnt * 6, 0);
		}
	}

	void Cleanup() {
		if(nullptr != m_pInst) {
			free(m_pInst);
			m_pInst = nullptr;
		}
		glDeleteVertexArrays(1, &m_VA);
		m_VA = 0;
		glDeleteBuffers(1, &m_IB);
		m_IB = 0;
	}

private:

private:
	bool m_bEnabled;
	GLuint m_VA; // Vertex Array
	GLuint m_IB; // Index Buffer
	int m_SpriteIndex;
	SpriteInst* m_pInst;
	PersistentBuffer<buffers> m_PBuffer;
};

#endif
