#include "Mesh.hpp"
#include "Output.h"
#include "OpenGL/Texture.h"

Mesh::Mesh() {
	m_DrawType = GL_TRIANGLES;
	m_VertCount = 0;
	m_Buffer = 0;
	m_VArray = 0;
	m_DrawCmd = 0;

	m_Texture = 0;
	m_BlankTex = 0;
}

Mesh::~Mesh() {
	Cleanup();
}

void Mesh::Draw() {
	GLERR;
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_DrawCmd);
	GL_ERROR();
	m_Transform.Bind(1);
	glBindVertexArray(m_VArray);
	GL_ERROR();
	glBindTextureUnit(0, m_Texture);
	GL_ERROR();
	glDrawArraysIndirect(m_DrawType, 0);
	GL_ERROR();
}

void Mesh::Cleanup() {
	glDeleteBuffers(1, &m_Buffer);
	m_Buffer = 0;
	glDeleteBuffers(1, &m_DrawCmd);
	m_DrawCmd = 0;
	glDeleteVertexArrays(1, &m_VArray);
	m_VArray = 0;
	glDeleteTextures(1, &m_BlankTex);
	m_BlankTex = 0;
}

void Mesh::SetTexture(GLuint id) {
	if(0 == id) {
		m_Texture = m_BlankTex;
	} else {
		m_Texture = id;
	}
}

Transform3D& Mesh::GetTransform() {
	return m_Transform;
}

void Mesh::CreateIndirectDraw(int vertcnt) {
	indirect_draw_t draw_cmd = CreateIndirectDrawCmd(vertcnt, 1, 0, 0);
	buffer_descriptor_t desc;
	desc.size = sizeof(indirect_draw_t);
	desc.data = &draw_cmd;
	desc.type = GL_STATIC_DRAW;
	CreateBuffer(desc, &m_DrawCmd);
}

void Mesh::CreateVertexBuffer(int cnt, TLVertex3D* verts) {
	CreateTL3DVertexBuffer(cnt, verts, 0, &m_Buffer, &m_VArray);
	m_VertCount = cnt;
}

void Mesh::CreateBlankTexture() {
	CreateEmptyTexture(&m_BlankTex, 0xffffffff);
}
