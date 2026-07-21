#ifndef SPRITEMANAGER_INCLUDED
#define SPRITEMANAGER_INCLUDED

#include "Sprite.hpp"
#include "Misc/Primitives.h"
#include "PersistentBuffer.hpp"
#include "TextureManager.hpp"

class SpriteManager {
public:
	SpriteManager();
	~SpriteManager();

	bool Init(int cnt, TextureManager* tex_man); // Max amount of accepted sprites
	void InitDrawList(); // Begin a draw list, current sprites = 0, enables the draw list
	sprite_inf_t* NextSprite(); // Gives a handle until reaching max
	void EndDrawList(); // Ends the draw list
	void Draw(); // Draw each sprite
		     //
	void Cleanup();
private:

private:
	sprite_inf_t* m_pSpriteData;
	bool m_bEnabled;
	int m_MaxSprites;
	int m_SpriteIndex;

	PersistentBuffer m_PBuffer;

	GLuint m_VB;
	GLuint m_VA;
	TextureManager* m_pTexMan;
};

#endif
