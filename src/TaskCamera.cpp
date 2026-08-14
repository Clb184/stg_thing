#include "TaskCamera.hpp"
#include "cstring"

TaskCamera::TaskCamera() {
	//pos = {0.0f, 0.0f, 0.0f};
	rot = {0.0f, 0.0f, 0.0f, 0.0f};
}

TaskCamera::~TaskCamera() {

}

bool TaskCamera::Init() {
	camera.Init();
	camera.SetAspectRatio(400.0f, 480.0f);
	buffer_descriptor_t desc;
	desc.size = sizeof(WorldLight);
	desc.data = nullptr;
	desc.type = GL_DYNAMIC_DRAW;
	CreateBuffer(desc, &wlight);
	return true;
}

void TaskCamera::Use() {
	camera.SetBinding(0);
	BindConstantBuffer(wlight, 2);
}

int XASM2CameraTask(uint8_t cmd, xasm2_vm_t* vm, float dt, void* data) {
	TaskCamera* camera = (TaskCamera*)data;
	switch(cmd) {
		case 0x80: // Add vector to pos
			DirectX::XMVECTOR dest = DirectX::XMLoadFloat4(&camera->light.cam_pos);
			DirectX::XMVECTOR add = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)vm->cmd);
			dest = DirectX::XMVectorAdd(dest, DirectX::XMVectorMultiply(add, DirectX::XMVectorReplicate(dt)));
			DirectX::XMStoreFloat4(&camera->light.cam_pos, dest);
			vm->cmd += sizeof(DirectX::XMFLOAT3);
			break;
	}
	return 0;
}

bool TaskCamera::Setup(uint8_t* script) {
	XASM2VMInit(&task, script);
	XASM2VMSetMembers(&task, 31, (int*)&light);
	return true;
}

void TaskCamera::Move(float dt) {
	XASM2Move(&task, dt, XASM2CameraTask, nullptr);
	camera.SetPos(light.cam_pos.x, light.cam_pos.y, light.cam_pos.z);
	camera.SetRot(rot.x, rot.y, rot.z);
	camera.SetFog(fog.x, fog.y);
	camera.SetFOV(fov);
	camera.Update();
	WorldLight* wl = (WorldLight*)glMapNamedBuffer(wlight, GL_WRITE_ONLY);
	memcpy(wl, &light, sizeof(WorldLight));
	glUnmapNamedBuffer(wlight);
}

