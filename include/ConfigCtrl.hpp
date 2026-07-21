#ifndef CONFIGCTRL_INCLUDED
#define CONFIGCTRL_INCLUDED

#include "Config.hpp"
#include "nlohmann/json.hpp"

class ConfigCtrl {
public:	
	ConfigCtrl();
	~ConfigCtrl();

	void SetLives(int cnt);
	int GetLives() const;

	void SetBombs(int cnt);
	int GetBombs() const;
		
	void SetSndVolume(int level);
	int GetSndVolume() const;

	void SetBGMVolume(int level);
	int GetBGMVolume() const;

	bool IsSoundEnabled() const;
	void SetSoundEnable(bool enable);
	bool IsBGMEnabled() const;
	void SetBGMEnable(bool enable);

	WINDOW_STATE GetWindowState() const;
	int GetWindowResolution() const;

	std::string GetFetchURL();

	void SaveConfig();

	void LoadDefaultConfig(); // 2 lives, 3 bombs, 80 vol for snd and BGM, 
private:
	bool ValidateJSON(const nlohmann::json& json_data);

private:
	game_config_t m_LoadedConfig;
};

#endif
