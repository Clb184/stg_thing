#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

#include "DirectXMath.h"
#include "OpenGL/Buffer.h"

class Camera {
public:
	Camera();
	~Camera();

	bool Init();

	void Update();
	void SetBinding(int idx);
	void SetPos(float x, float y, float z);
	void SetRot(float pitch, float yaw, float roll);
	
	void SetAspectRatio(float w, float h);
	void SetFOV(float fov);

	void SetFog(float near, float far);

private:
	struct CameraData {
		DirectX::XMMATRIX cam;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMFLOAT4 fog = {100.0f, 150.0f, 0.0f, 0.0f};
		DirectX::XMFLOAT4 camera_pos = {0.5f, 0.0f, 0.0f, 0.0f};
		DirectX::XMFLOAT4 extra = {0.0f, 0.0f, 0.0f, 0.0f};
	};

	// In the strange case I ever decide to look at a fixed point with the camera (?
	bool m_bLookAt;
	float m_LookAtPos[4];
	
	DirectX::XMFLOAT3 m_CamPos;
	DirectX::XMFLOAT3 m_CamRot;

	DirectX::XMFLOAT2 m_FogPos;
	DirectX::XMFLOAT2 m_ExpFog;
	float m_FogDensity;

	float m_Ratio;
	float m_FOV;

	GLuint m_Buffer;
};

#endif
