#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "OpenGL/Buffer.h"
#include "DirectXMath.h"

// 3D Mesh base class
class Mesh {
public:
	Mesh();
	~Mesh();

	void Draw();
	void Cleanup();

private:
	int m_DrawType;
	int m_VertCount;
	GLuint m_Buffer;
	GLuint m_VArray;
	GLuint m_DrawCmd;
};


#endif
