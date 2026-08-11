#include "Transform3D.hpp"

Transform3D::Transform3D() {
	m_Buffer = 0;
	m_Pos = {0.0f, 0.0f, 0.0f};
	m_Rot = {0.0f, 0.0f, 0.0f};
	m_Scale = {1.0f, 1.0f, 1.0f};
}

Transform3D::~Transform3D() {
	glDeleteBuffers(1, &m_Buffer);
	m_Buffer = 0;
}

void Transform3D::Init() {
	DirectX::XMMATRIX model = DirectX::XMMatrixIdentity();
	buffer_descriptor_t desc;
	desc.size = sizeof(DirectX::XMMATRIX);
	desc.data = &model;
	desc.type = GL_DYNAMIC_DRAW;
	CreateBuffer(desc, &m_Buffer);
}

void Transform3D::Update() {
	DirectX::XMMATRIX model = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z) * DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_Scale)) * DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_Rot));

	DirectX::XMMATRIX* buf = (DirectX::XMMATRIX*)glMapNamedBuffer(m_Buffer, GL_WRITE_ONLY);
	*buf = model;
	glUnmapNamedBuffer(m_Buffer);
}

void Transform3D::Bind(int slot) {
	BindConstantBuffer(m_Buffer, slot);
}

void Transform3D::SetPos(float x, float y, float z) {
	m_Pos = {x, y, z};
}

void Transform3D::SetRot(float pitch, float yaw, float roll) {
	m_Rot = {pitch, yaw, roll};
}

void Transform3D::SetScale(float x, float y, float z) {
	m_Scale = { x, y, z };
}
