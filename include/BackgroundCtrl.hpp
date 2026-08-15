#ifndef BACKGROUNDCTRL_INCLUDED
#define BACKGROUNDCTRL_INCLUDED

#include "TaskCamera.hpp"
#include "InputDevice.hpp"
#include "OpenGL/Texture.h"
#include "TextureManager.hpp"
#include "3DPlane.hpp"

class BackgroundCtrl {
private:
public:
	BackgroundCtrl();
	~BackgroundCtrl();
	
	void SetDebugControl(InputDevice* input);

	void Init(TextureManager* texman);
	GLuint GetTexture() const;
	void SetupTask(uint8_t* script);
	void Move(float dt);
	void Draw();

	TaskCamera& GetCameraTask();
private:
	void MoveInput(float dt);
private:
	render_texture_t m_PPBG;
	TaskCamera m_Camera;
	Plane3D m_Plane;

	TextureManager* m_pTexMan;
	InputDevice* m_pInput;
};

#endif
