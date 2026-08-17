#include "SoundCtrl.hpp"
#include "Output.h"
#include "PackArchive/PackFile.h"

SoundCtrl::SoundCtrl() {
	m_bBGMOn = true;
	m_bSFXOn = true;
}

SoundCtrl::~SoundCtrl() {
	Cleanup();
}

bool SoundCtrl::Init(GameInfo* info, ConfigCtrl* cfg, ScreenOutput* out) {
	LOG_INFO("Initializing SoundControl");
	auto& sounds = info->GetSoundEntries();
	bool ret = false;
	pack_file_t pack;
	Cleanup();

	m_Out = out;
	m_pInfo = info;
	LOG_INFO("Initializing SoundControl");
	if(ret = InitializeSoundControl(&m_SndCtrl, sounds.size())) {
		std::string dat = info->GetArchive();
		if(0 == PackFileOpen(&pack, dat.c_str())) {
			LOG_INFO("Opened pack \"" + dat + "\"");
			int i = 0;
			for(auto& snd : sounds) {
				char* data = 0;
				size_t size = 0;
				std::string snd_name = std::get<0>(snd);
				if(0 == PackFileLoadEntry(&pack, snd_name.c_str(), (void**)&data, &size)) {
					if(false == LoadSoundFromMemory(&m_SndCtrl, i, std::get<1>(snd), data, size)) {
						m_Out->LogError("Error loading sound \"" + snd_name + "\"");
					}
					else {
						m_Out->LogInfo("Loaded sound \"" + snd_name + "\"");
					}
					free(data);
				}
				i++;
			}
			ret = true;
			PackFileClose(&pack);
		}
		else {
			LOG_ERROR("Failed opening pack");
			ret = false;
		}
		ApplyConfig(cfg);
	}
	return ret;
}

void SoundCtrl::ApplyConfig(ConfigCtrl* cfg) {
	SetSoundMasterVolume(&m_SndCtrl, float(cfg->GetMasterVolume()) * 0.01f);
	SetSoundMusicVolume(&m_SndCtrl, float(cfg->GetSndVolume()) * 0.01f, float(cfg->GetBGMVolume()) * 0.01f);
	m_bBGMOn = cfg->IsBGMEnabled();
	m_bSFXOn = cfg->IsSoundEnabled();
}

void SoundCtrl::Cleanup() {
	DestroySoundControl(&m_SndCtrl);
}

void SoundCtrl::PlaySound(int idx) {
	PlaySnd(&m_SndCtrl, idx);
}

void SoundCtrl::PlaySoundX(int idx, float x) {
	PlaySndX(&m_SndCtrl, idx, x);
}

void SoundCtrl::MusicLoad(int idx) {
	::MusicStop(&m_SndCtrl);
	pack_file_t pack;
	std::string dat = m_pInfo->GetArchive();
	if(0 == PackFileOpen(&pack, dat.c_str())) {
		m_Out->LogInfo("Opened pack \"" + dat + "\"");
		char* data = 0;
		size_t size = 0;
		auto bgm_dat = m_pInfo->GetMusicInfo(idx);
		std::string bgm = std::get<0>(bgm_dat);
		if(bgm == "") {
			PackFileClose(&pack);
			m_Out->LogError("Unknown BGM id");
			return;
		}
		if(0 == PackFileLoadEntry(&pack, bgm.c_str(), (void**)&data, &size)) {
			if(0 != LoadMusicFromMemory(&m_SndCtrl, data, size)) {
				m_Out->LogError("Failed loading BGM \"" + bgm + "\"");	
			} else {
				m_Out->LogInfo("Loaded BGM \"" + bgm + "\"");
				MusicEnableLoop(&m_SndCtrl, 1);
				MusicSetLoop(&m_SndCtrl, std::get<1>(bgm_dat), std::get<2>(bgm_dat));
			}
			free(data);
		}
		else {
			m_Out->LogError("BGM \"" + bgm + "\" not found in pack");
		}
		PackFileClose(&pack);
	}
	else {
		m_Out->LogError("Failed opening pack");
	}
	
}

void SoundCtrl::MusicPlay() {
	::MusicPlay(&m_SndCtrl);
}

void SoundCtrl::MusicPause() {
	::MusicPause(&m_SndCtrl);
}

void SoundCtrl::MusicStop() {
	::MusicStop(&m_SndCtrl);
}

