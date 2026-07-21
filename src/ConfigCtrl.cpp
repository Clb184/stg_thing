#include "ConfigCtrl.hpp"
#include "IO.h"
#include "Output.h"

const char* g_ConfigName = "config.json";

ConfigCtrl::ConfigCtrl() {
	LOG_INFO("Loading Config file");
	char* source = nullptr;
	nlohmann::json config;

	if(true == LoadTextFromFile(g_ConfigName, &source, 0)) {
		LOG_INFO("Parsing JS");
		config = nlohmann::json::parse(source);
		LOG_INFO("Freeing source JSON");
		free(source);
		if(true == ValidateJSON(config)) {
			LOG_INFO("Success on loading config");
			m_LoadedConfig.playername = config["playername"];
			m_LoadedConfig.lives = config["lives"];
			m_LoadedConfig.bombs = config["bombs"];
			m_LoadedConfig.sndvol = config["sndvol"];
			m_LoadedConfig.snd_enable = config["snden"];
			m_LoadedConfig.bgmvol = config["bgmvol"];
			m_LoadedConfig.bgm_enable = config["bgmen"];
			m_LoadedConfig.win_state = (WINDOW_STATE)(int)config["window"];
			m_LoadedConfig.resolution = config["resolution"];
			m_LoadedConfig.fetch_url = config["fetch_url"];
			return;
		}
	}
	LOG_ERROR("Failed loading config");
	LoadDefaultConfig();
}

ConfigCtrl::~ConfigCtrl() {

}

void ConfigCtrl::SetLives(int cnt) {
	m_LoadedConfig.lives = cnt;
}

int ConfigCtrl::GetLives() const {
	return m_LoadedConfig.lives;
}

void ConfigCtrl::SetBombs(int cnt) {
	m_LoadedConfig.bombs = cnt;	
}

int ConfigCtrl::GetBombs() const {
	return m_LoadedConfig.bombs;
}
		
void ConfigCtrl::SetSndVolume(int level) {
	m_LoadedConfig.sndvol = level;
}

int ConfigCtrl::GetSndVolume() const {
	return m_LoadedConfig.sndvol;
}

void ConfigCtrl::SetBGMVolume(int level) {
	m_LoadedConfig.bgmvol = level;
}

int ConfigCtrl::GetBGMVolume() const {
	return m_LoadedConfig.bgmvol;
}

bool ConfigCtrl::IsSoundEnabled() const {
	return m_LoadedConfig.snd_enable;
}

void ConfigCtrl::SetSoundEnable(bool enable) {
	m_LoadedConfig.snd_enable = enable;
}

bool ConfigCtrl::IsBGMEnabled() const {
	return m_LoadedConfig.bgm_enable;
}

void ConfigCtrl::SetBGMEnable(bool enable) {
	m_LoadedConfig.bgm_enable = enable;
}

WINDOW_STATE ConfigCtrl::GetWindowState() const {
	return m_LoadedConfig.win_state;
}

int ConfigCtrl::GetWindowResolution() const {
	return m_LoadedConfig.resolution;
}

std::string ConfigCtrl::GetFetchURL() {
	return m_LoadedConfig.fetch_url;
}

void ConfigCtrl::SaveConfig() {
	LOG_INFO("Saving configuration");
	nlohmann::json save_cfg;
	save_cfg["playername"] = m_LoadedConfig.playername;
	save_cfg["lives"] = m_LoadedConfig.lives;
	save_cfg["bombs"] = m_LoadedConfig.bombs;
	save_cfg["sndvol"] = m_LoadedConfig.sndvol;
	save_cfg["snden"] = m_LoadedConfig.snd_enable;
	save_cfg["bgmvol"] = m_LoadedConfig.bgmvol;
	save_cfg["bgmen"] = m_LoadedConfig.bgm_enable;
	save_cfg["window"] = (int)m_LoadedConfig.win_state;
	save_cfg["resolution"] = m_LoadedConfig.resolution;
	save_cfg["fetch_url"] = m_LoadedConfig.fetch_url;
	save_cfg["0"] = "Any extra keys or incorrect values will reset the config, be careful";

#ifdef NDEBUG
#endif
	fprintf(stdout, "JSON CONFIG: \n%s\n", save_cfg.dump(4).c_str());
	FILE* fp = fopen(g_ConfigName, "w");
	if(0 != fp) {
		const std::string str = save_cfg.dump(4);
		fwrite(str.c_str(), str.length(), 1, fp);
		fclose(fp);
	}
	else {
		LOG_ERROR("Failed saving config file\n");
	}
}

void ConfigCtrl::LoadDefaultConfig() {
	LOG_INFO("Loading default configuration");
	m_LoadedConfig.playername = "Player";
	m_LoadedConfig.lives = 2;
	m_LoadedConfig.bombs = 3;
	m_LoadedConfig.sndvol = 80;
	m_LoadedConfig.snd_enable = true;
	m_LoadedConfig.bgmvol = 80;
	m_LoadedConfig.bgm_enable = true;
	m_LoadedConfig.win_state = WS_WINDOW;
	m_LoadedConfig.resolution = 4; // Start at 1280x960
	m_LoadedConfig.fetch_url = "192.168.0.63:5000";
}

bool ConfigCtrl::ValidateJSON(const nlohmann::json& js) {
	LOG_INFO("Validating config JSON");
	// Validate string:
	if(js.find("playername") == js.end() || js.find("lives") == js.end() || js.find("bombs") == js.end() || js.find("sndvol") == js.end() || js.find("bgmvol") == js.end() || js.find("window") == js.end() || js.find("resolution") == js.end() || js.find("snden") == js.end() || js.find("bgmen") == js.end() || js.find("fetch_url") == js.end()) return false;
	try {
		std::string str = js["playername"];
		str = js["fetch_url"];
		
		int integer = 0;
		integer = js["lives"];
		integer = js["bombs"];
		integer = js["sndvol"];
		integer = js["bgmvol"];
		integer = js["window"];
		integer = js["resolution"];

		bool boolean = true;
		boolean = js["snden"];
		boolean = js["bgmen"];
	}
	catch(...) {
		return false;
	}
	return true;
}
