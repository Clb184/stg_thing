#ifndef SPRITE_INCLUDED
#define SPRITE_INCLUDED

#include "cstdint"
#include "GL/glew.h"

struct sprite_inf_t {
	int tex_id;
	float ang;
	float x, y;
	float w, h;
	float sx, sy;
	uint32_t color;
	float u0, v0, u1, v1;
};

class Sprite {
public:
	Sprite();
	~Sprite();
	
	bool Init();
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
