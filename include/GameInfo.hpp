#ifndef GAMEINFO_INCLUDED
#define GAMEINFO_INCLUDED

#include "utility"
#include "string"
#include "vector"

class GameInfo {
public:
	typedef std::tuple<std::string, uint64_t, uint64_t> MusicInfo;
	typedef std::tuple<std::string, int> SoundEntry;
public:
	GameInfo();
	~GameInfo();

	bool Init();
	void Reload();
	
	const std::string& GetArchive() const;
	MusicInfo GetMusicInfo(int id) const;
	const std::vector<SoundEntry>& GetSoundEntries();
	const std::string& GetTitle() const;
	const std::string& GetTestLevel() const;

private:
	void Cleanup();

private:
	std::string m_Archive;
	std::string m_Title;
	std::vector<MusicInfo> m_MusicInfo;
	std::vector<SoundEntry> m_SoundEntries;
	std::vector<std::string> m_LevelNames;
	std::string m_TestLevel;

};

#endif
