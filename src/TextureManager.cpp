#include "TextureManager.hpp"
#include "Output.h"
#include "cassert"

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {
	Cleanup();
}

void TextureManager::Init() {
	Cleanup();
}

GLuint TextureManager::LoadTexture(char* data, size_t size) {
	LOG_INFO("Loading texture...");
	texture_inf_t inf;
	bool result = LoadTextureFromMemory(data, &inf.unit, &inf.metrics);
	if(false == result) {
		CreateEmptyTexture(&inf.unit, 0xff808080);
		inf.metrics.width = 256;
		inf.metrics.height = 256;
		inf.metrics.texelw = inf.metrics.texelh = 1.0f / 256.0f;
		LOG_ERROR("Failed loading texture");
	}
	GLuint ret = inf.unit;
	m_Textures.emplace_back(std::move(inf));
	return ret;
}

GLuint TextureManager::GetTextureID(int idx) {
	if(idx >= m_Textures.size()) return -1;
	return m_Textures[idx].unit;
}

texture_metric_t TextureManager::GetTextureMetrics(int id) {
	if(id >= m_Textures.size()) return  { 0 };
	return m_Textures[id].metrics;
}

void TextureManager::Cleanup() {
	for(const auto& t : m_Textures) {
		glDeleteTextures(1, &t.unit);
	}
	m_Textures.clear();
}
