#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#include "cstdint"
#include "string"

enum WINDOW_STATE {
	WS_FULLSCREEN,
	WS_WINDOW,
	WS_BORDERLESS
};

struct game_config_t {
	std::string playername;
	int lives;
	int bombs;
	int sndvol;
	bool snd_enable;
	int bgmvol;
	bool bgm_enable;
	WINDOW_STATE win_state;
	int resolution;
	std::string fetch_url;
};

#endif
