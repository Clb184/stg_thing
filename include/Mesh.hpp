#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "OpenGL/Buffer.h"
#include "Misc/Primitives.h"
#include "Transform3D.hpp"

// 3D Mesh base class
class Mesh {
public:
	Mesh();
	~Mesh();

	void Draw();
	void Cleanup();
	void SetTexture(GLuint id);
	Transform3D& GetTransform();
private:
	friend class Plane3D;
	void CreateIndirectDraw(int vertcnt);
	void CreateVertexBuffer(int cnt, TLVertex3D* verts);
	void CreateBlankTexture();
private:
	int m_DrawType;
	int m_VertCount;
	GLuint m_Buffer;
	GLuint m_VArray;
	GLuint m_DrawCmd;
	GLuint m_Texture;

	GLuint m_BlankTex;

	Transform3D m_Transform;
};


#endif
