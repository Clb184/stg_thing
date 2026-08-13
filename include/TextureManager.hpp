#ifndef TEXTUREMANAGER_INCLUDED
#define TEXTUREMANAGER_INCLUDED

#include "OpenGL/Texture.h"
#include "PackArchive/PackFile.h"
#include "ScreenOutput.hpp"
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
	
	void Init(ScreenOutput* IO);
	GLuint Load(const char* file);
	GLuint LoadPack(pack_file_t* pack_file, const char* entry_name);
	GLuint LoadEx(char* data, size_t size);
	GLuint GetTextureID(int id);
	texture_metric_t GetTextureMetrics(int id);
	void Cleanup();

private:
	std::vector<texture_inf_t> m_Textures;
	ScreenOutput* m_Out;
};

#endif
