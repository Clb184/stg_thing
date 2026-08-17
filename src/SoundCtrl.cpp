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

bool SoundCtrl::Initialize(GameInfo* info, ConfigCtrl* cfg, ScreenOutput* out) {
	LOG_INFO("Initializing SoundControl");
	auto& sounds = info->GetSoundEntries();
	bool ret = false;
	pack_file_t pack;
	Cleanup();

	m_Out = out;
	if(ret = InitializeSoundControl(&m_SndCtrl, sounds.size())) {
		std::string dat = info->GetArchive();
		if(0 == PackFileOpen(&pack, dat.c_str())) {
			m_Out->LogInfo("Opened pack \"" + dat + "\"");
			int i = 0;
			for(std::tuple<std::string, int>& snd : sounds) {
				char* data = 0;
				size_t size = 0;
				std::string snd_name = std::get<0>(snd);
				if(0 == PackFileLoadEntry(&pack, snd_name.c_str(), (void**)&data, &size)) {
					if(false == LoadSoundFromMemory(&m_SndCtrl, i, std::get<1>(snd), data, size)) {
						m_Out->LogError("Error loading sound \"" + snd_name + "\"");
					}
					free(data);
				}
				i++;
			}
			PackFileClose(&pack);
		}
		else {
			m_Out->LogError("Failed opening pack");
		}
	}
	ApplyConfig(cfg);
	return true;
}

void SoundCtrl::ApplyConfig(ConfigCtrl* cfg) {
	SetSoundMasterVolume(float(cfg->GetMasterVolume()) * 0.01f);
	SetSoundMusicVolume(&m_SndCtrl, float(cfg->GetSndVolume()) * 0.01f, float(cfg->GetBGMVolume()) * 0.01f);
	m_bBGMOn = cfg->IsBGMEnabled();
	m_bSFXOn = cfg->IsSoundEnabled();
}

void SoundCtrl::Cleanup() {
	DestroySoundControl(&m_SndCtrl);
}
