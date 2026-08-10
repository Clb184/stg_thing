#include "Mesh.hpp"

Mesh::Mesh() {
	m_DrawType = GL_TRIANGLES;
	m_VertCount = 0;
	m_Buffer = 0;
	m_VArray = 0;
	m_DrawCmd = 0;
}

Mesh::~Mesh() {
	Cleanup();
}

void Mesh::Draw() {
	GLERR;
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_DrawCmd);
	GL_ERROR();
	glBindVertexArray(m_VArray);
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
}
