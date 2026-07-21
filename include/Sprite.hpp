#ifndef SPRITE_INCLUDED
#define SPRITE_INCLUDED

#include "cstdint"
#include "GL/glew.h"

struct sprite_inf_t {
	GLuint tex_id = 0;
	float ang = 0.0f;
	float x = 0.0f, y = 0.0f;
	float w = 16.0f, h = 16.0f;
	float sx = 1.0f, sy = 1.0f;
	uint32_t color = 0xffffffff;
	float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
};

class Sprite {
public:
	Sprite();
	~Sprite();
	
	bool Init();
	void SetTexID(GLuint id);
	void SetPos(float x, float y);
	void SetAngle(float dir);
	void SetSize(float w, float h);
	void SetScale(float sx, float sy);
	void SetUV(float u0, float v0, float u1, float v1);
	void SetColor(uint32_t color);
	void Draw();

private:
	bool m_bNeedUpdate;
	sprite_inf_t m_SpriteInf;
	GLuint m_VB;
	GLuint m_VA;
};

#endif
