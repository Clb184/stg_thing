#include "Transform3D.hpp"

Transform3D::Transform3D() {
	DirectX::XMMATRIX model = DirectX::XMMatrixIdentity();
	buffer_descriptor_t desc;
	desc.size = sizeof(DirectX::XMMATRIX);
	desc.data = &model;
	desc.type = GL_DYNAMIC_DRAW;
	CreateBuffer(desc, &m_Buffer);
}

Transform3D::~Transform3D() {
	glDeleteBuffers(1, &m_Buffer);
	m_Buffer = 0;
}

Transform3D::Update() {
	DirectX::XMMATRIX model = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z) * DirectX::XMMatrixRotationPitchYawFromVector(DirectX::XMLoadFloat3(&m_Rot)) * DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_Scale));
}


