#ifndef TRANSFORM3D_INCLUDED
#define TRANSFORM3D_INCLUDED

#include "OpenGL/Buffer.h"
#include "DirectXMath.h"

class Transform3D {
public:
	Transform3D();
	~Transform3D();

	void Update();
	void Bind(int slot);
	void SetPos(float x, float y, float z);
	void SetRot(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);
private:
	GLuint m_Buffer; // Where the uniform data is
	DirectX::XMFLOAT3 m_Pos;
	DirectX::XMFLOAT3 m_Rot;
	DirectX::XMFLOAT3 m_Scale;
};
#endif
