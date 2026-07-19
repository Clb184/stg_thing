#ifndef SPRITEMANAGER_INCLUDED
#define SPRITEMANAGER_INCLUDED

#include "Sprite.hpp"

class SpriteManager {
public:
	SpriteManager();
	~SpriteManager();

	bool Init(int cnt); // Max amount of accepted sprites
	void InitDrawList(); // Begin a draw list, current sprites = 0, enables the draw list
	sprite_inf_t* NextSprite(); // Gives a handle until reaching max
	void EndDrawList(); // Ends the draw list
	void Present(); // Draw each sprite

private:
	sprite_inf_t* m_pSpriteData;
	bool m_bEnabled;
	int m_MaxSprites;
	int m_SpriteIndex;
	//int m_CurrentSprites;


};

#endif
