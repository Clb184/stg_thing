#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "OpenGL/Buffer.h"
#include "Transform3D.hpp"

// 3D Mesh base class
class Mesh {
public:
	Mesh();
	~Mesh();

	void Draw();
	void Cleanup();

private:
	void CreateIndirectDraw(int vertcnt);

private:
	int m_DrawType;
	int m_VertCount;
	GLuint m_Buffer;
	GLuint m_VArray;
	GLuint m_DrawCmd;

	Transform3D m_Transform;
};


#endif
