#ifndef PKGMANAGER_INCLUDED
#define PKGMANAGER_INCLUDED

#include "iostream"
#include "httplib.h"
#include "nlohmann/json.hpp"

class PkgManager {
public:
	PkgManager();
	~PkgManager();

	bool Init();
	void SetFetchURL(const std::string& url);
	bool Refresh();

	int GetPkgCount() const;
	std::string GetEntryName(int id);
	std::string GetEntryAuthor(int id);
	std::string GetPkgDir(int id);
	bool GetAvailable(int id);
	int GetEntryID(int i);
	void DownloadEntry(int id);
	
	bool IsListReady();

	void Cleanup();
	void SaveCache();

private:
	void LoadDownloadedCache();
	void LoadPackageCache();

	void WriteDownloadedCache();
	void WritePackageCache();

private:
	nlohmann::json m_Entries;
	nlohmann::json m_Downloaded;
	std::string m_URL;
	bool m_bReady;
};

#endif
