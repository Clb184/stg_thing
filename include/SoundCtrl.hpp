#ifndef SOUNDCTRL_INCLUDED
#define SOUNDCTRL_INCLUDED

#include "Miniaudio/Sound.h"
#include "GameInfo.hpp"
#include "ScreenOutput.hpp"
#include "ConfigCtrl.hpp"

class SoundCtrl {
public:
	SoundCtrl();
	~SoundCtrl();

	bool Init(GameInfo* info, ConfigCtrl* cfg, ScreenOutput* out);
	void ApplyConfig(ConfigCtrl* cfg);
	void Cleanup();
	
	// Sound related
	void PlaySound(int idx);
	void PlaySoundX(int idx, float x); // Pos ranges from -300 to 300

	// Music related
	void MusicLoad(int idx);
	void MusicPlay();
	void MusicPause();
	void MusicStop();

private:
	bool m_bBGMOn;
	bool m_bSFXOn;
	sound_control_t m_SndCtrl;
	ScreenOutput* m_Out;
	GameInfo* m_pInfo;
};

#endif
