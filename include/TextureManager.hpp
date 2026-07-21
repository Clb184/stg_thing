#ifndef TEXTUREMANAGER_INCLUDED
#define TEXTUREMANAGER_INCLUDED

#include "OpenGL/Texture.h"
#include "vector"

class TextureManager {
private:
	struct texture_inf_t {
		GLuint unit;
		texture_metric_t metrics;
	};
public:
	TextureManager();
	~TextureManager();
	
	void Init();
	GLuint LoadTexture(char* data, size_t size);
	GLuint GetTextureID(int id);
	texture_metric_t GetTextureMetrics(int id);
	void Cleanup();

private:
	std::vector<texture_inf_t> m_Textures;
};

#endif
