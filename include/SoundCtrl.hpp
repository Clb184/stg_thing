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

	bool Initialize(GameInfo* info, ConfigCtrl* cfg, ScreenOutput* out);
	void ApplyConfig(ConfigCtrl* cfg);
	void Cleanup();
	
	// Sound related
	void PlaySound(int idx);

	// Music related
	void MusicPlay(int idx);
	void MusicResume();
	void MusicPause();
	void MusicStop();

private:
	bool m_bBGMOn;
	bool m_bSFXOn;
	sound_ctrl_t m_SndCtrl;
	ScreenOutput* m_Out;
};

#endif
