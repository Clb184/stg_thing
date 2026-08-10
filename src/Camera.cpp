#include "Camera.hpp"
#include "cstring"
#include "Output.h"

Camera::Camera() {
	m_bLookAt = false;
	memset(m_LookAtPos, 0, sizeof(float) * 4);
	//memset(m_CamPos, 0, sizeof(float) * 4);
	//memset(m_CamRot, 0, sizeof(float) * 4);

	m_FogPos = {50.0f, 70.0f};
	m_ExpFog = {0.0f, 0.0f};
	m_FogDensity = 0.0f;

	m_Ratio = 4.0f / 3.0f;
	m_FOV = 3.14159 * 0.25f;

	m_Buffer = 0;
}

Camera::~Camera() {

}

bool Camera::Init() {
	LOG_INFO("Initializing Camera");
	CameraData init;
	init.cam = DirectX::XMMatrixIdentity();
	init.view = DirectX::XMMatrixIdentity();
	init.proj = DirectX::XMMatrixIdentity();
	init.fog.x = m_FogPos.x;
	init.fog.y = m_FogPos.y;

	buffer_descriptor_t desc;
	desc.size = sizeof(CameraData);
	desc.data = &init;
	desc.type = GL_DYNAMIC_DRAW;

	if(false == CreateBuffer(desc, &m_Buffer)) {
		LOG_INFO("Failed creating Camera constant buffer");
		return false;
	}
	return true;
}

void Camera::Update() {
	CameraData cam;
	cam.fog.x = m_FogPos.x;
	cam.fog.y = m_FogPos.y;
	cam.camera_pos.x = m_CamPos.x;
	cam.camera_pos.y = m_CamPos.y;
	cam.camera_pos.z = m_CamPos.z;

	// Rotate using quaternions
	DirectX::XMVECTOR rot = DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&m_CamRot), _mm_set_ps1(0.5f)); // Load and get the half already to use it with quaternions
	DirectX::XMVECTOR sins = DirectX::XMVectorSin(rot);// = _mm_sin_ps(rot); //Calculate sin of all axis
	DirectX::XMVECTOR coss = DirectX::XMVectorCos(rot);// = _mm_cos_ps(rot); //Same for cos
	
	DirectX::XMFLOAT3 rts, rtc; //Sine and cosine Obtained
	DirectX::XMStoreFloat3(&rts, sins);
	DirectX::XMStoreFloat3(&rtc, coss);
	DirectX::XMVECTOR q = DirectX::XMQuaternionMultiply(DirectX::XMVectorSet(rts.z, 0.0f, 0.0f, rtc.z), DirectX::XMVectorSet(0.0f, 0.0f, rts.x, rtc.x)); // Get the rotation of camera
	DirectX::XMVECTOR qy = DirectX::XMVectorSet(0.0f, rts.y, 0.0f, rtc.y);
	DirectX::XMVECTOR o = DirectX::XMVectorNegate(DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationQuaternion(q)));
	DirectX::XMFLOAT4 up = {0.0f, 0.0f, 1.0f, 1.0f};
	DirectX::XMVECTOR u = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&up), DirectX::XMMatrixRotationQuaternion(qy));
	// Update matrix
	DirectX::XMVECTOR eye_pos = DirectX::XMLoadFloat3(&m_CamPos);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(eye_pos, DirectX::XMVectorSetZ(o, -DirectX::XMVectorGetZ(o)), u);
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_Ratio, 0.1f, 1000.0f);
	cam.cam = view * projection;
	cam.view = view;
	cam.proj = projection;

	CameraData* dat = (CameraData*)glMapNamedBuffer(m_Buffer, GL_WRITE_ONLY);
	if(nullptr != dat) {
		memcpy(dat, &cam, sizeof(CameraData));
		glUnmapNamedBuffer(m_Buffer);
	}
}

void Camera::SetBinding(int idx) {
	BindConstantBuffer(m_Buffer, idx);
}

void Camera::SetPos(float x, float y, float z) {
	m_CamPos = {x, -y, z };
}

void Camera::SetRot(float pitch, float yaw, float roll) {
	m_CamRot = {pitch, yaw, roll};
}

void Camera::SetAspectRatio(float width, float height) {
	m_Ratio = width / height;
}

void Camera::SetFOV(float radians) {
	m_FOV = radians;
}

void Camera::SetFog(float near, float far) {
	m_FogPos = {near, far};
}
