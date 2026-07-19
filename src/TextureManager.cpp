#include "TextureManager.hpp"
#include "Output.h"
#include "cassert"

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {

}

bool TextureManager::LoadTexture(char* data, size_t size) {
	return true;
}

GLuint TextureManager::GetTextureID(int id) {
	if(id >= m_Textures.size()) return -1;
	return m_Textures[id].unit;
}

texture_metric_t TextureManager::GetTextureMetrics(int id) {
	if(id >= m_Textures.size()) return  { 0 };
	return m_Textures[id].metrics;
}

void TextureManager::Cleanup() {
	for(const auto& t : m_Textures) {
		glDeleteTextures(1, &t.unit);
	}
}
