#include "GameInfo.hpp"
#include "nlohmann/json.hpp"
#include "IO.h"
#include "iostream"

GameInfo::GameInfo() {

}

GameInfo::~GameInfo() {
	Cleanup();
}

bool GameInfo::Init() {
	char* data;
	size_t size;
	Cleanup();
	if(LoadTextFromFile("GAME.JSON", &data, &size)) {
		try {
			nlohmann::json info = nlohmann::json::parse(data);

			// Pack file key
			if(info.find("archive") != info.end()) m_Archive = info["archive"];
			else throw "Archive key not found";

			// Title key
			if(info.find("title") != info.end()) m_Title = info["title"];
			else m_Title = "STG";

			// BGM info key
			if(info.find("bgm") != info.end()) {
				std::vector<MusicInfo> bgm = info["bgm"];
				for(auto& entry : bgm) {
					std::cout << "BGM name: " << std::get<0>(entry) << "\n";
				}
				m_MusicInfo = std::move(bgm);
			}

			// Sound entries key
			if(info.find("sfx") != info.end()) {
				std::vector<SoundEntry> sfx = info["sfx"];
				for(auto& entry : sfx) {
					std::cout << "SFX name: " << std::get<0>(entry) << "\n";
				}
				m_SoundEntries = std::move(sfx);
			}

			// Level names
			if(info.find("main_levels") != info.end()) {
				std::vector<std::string> levels = info["main_levels"];
				for(auto& entry : levels) {
					std::cout << "Level name: " << entry << "\n";
				}
				m_LevelNames = std::move(levels);
			}

			// Demo level
			if(info.find("test_level") != info.end()) {
				std::string test = info["test_level"];
				std::cout << "Test level: " << test << "\n";
				m_TestLevel = test;
			}

		}
		catch(std::exception& e) {
			std::cout << "GAMEINFO EXCEPTION: " << e.what() << "\n";
			return false;	
		}
		free(data);
	}
	return true;
}

void GameInfo::Reload() {
	Init();
}

const std::string& GameInfo::GetArchive() const {
	return m_Archive;
}

GameInfo::MusicInfo GameInfo::GetMusicInfo(int id) const {
	if(id < m_MusicInfo.size()) return m_MusicInfo[id];
	else return std::make_tuple("", 0, 0);
}

const std::vector<GameInfo::SoundEntry>& GameInfo::GetSoundEntries() {
	return m_SoundEntries;
}

const std::string& GameInfo::GetTitle() const {
	return m_Title;
}

const std::string& GameInfo::GetTestLevel() const {
	return m_TestLevel;
}

void GameInfo::Cleanup() {
	m_MusicInfo.clear();
	m_SoundEntries.clear();
	m_LevelNames.clear();
}
