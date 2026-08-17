#include "TextureManager.hpp"
#include "Output.h"
#include "cassert"

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {
	Cleanup();
}

void TextureManager::Init(ScreenOutput* IO) {
	Cleanup();
	m_Out = IO;
}

GLuint TextureManager::Load(const char* name) {
	LOG_INFO("Loading texture (name)...");
	texture_inf_t inf;
	m_Out->LogInfo("Loading texture \"" + std::string(name) + "\"");
	bool result = LoadTextureFromFile(name, &inf.unit, &inf.metrics);
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

GLuint TextureManager::LoadP(pack_file_t* pack_file, const char* entry) {
	LOG_INFO("Loading texture from pack file...");
	char* data = nullptr;
	size_t size = 0;
	texture_inf_t inf;
	bool result = false;
	bool loaded = false;
	if(0 == PackFileLoadEntry(pack_file, entry, (void**)&data, &size)) {
		loaded = true;
		m_Out->LogInfo("Loading texture from pack \"" + std::string(entry) + "\"");
		result = LoadTextureFromMemory(data, &inf.unit, &inf.metrics);
	} else {
		m_Out->LogError("Texture not found in pack file");
	}
	if(false == result) {
		CreateEmptyTexture(&inf.unit, 0xff808080);
		inf.metrics.width = 256;
		inf.metrics.height = 256;
		inf.metrics.texelw = inf.metrics.texelh = 1.0f / 256.0f;
		LOG_ERROR("Failed loading texture");
	}
	GLuint ret = inf.unit;
	m_Textures.emplace_back(std::move(inf));
	if(true == loaded) {
		free(data);
	}
	return ret;
}

GLuint TextureManager::LoadEx(char* data, size_t size) {
	LOG_INFO("Loading texture from memory...");
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

int TextureManager::GetCount() const {
	return m_Textures.size();
}

void TextureManager::Cleanup() {
	for(const auto& t : m_Textures) {
		glDeleteTextures(1, &t.unit);
	}
	m_Textures.clear();
}
