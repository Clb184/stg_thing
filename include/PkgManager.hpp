#ifndef PKGMANAGER_INCLUDED
#define PKGMANAGER_INCLUDED

#include "iostream"
#ifdef WIN32
#define _WIN32_WINDOWS 0x0a00
#endif
#include "httplib.h"

#include "curl/curl.h"

/*
#define ASIO_STANDALONE
#include "asio.hpp"
#include "asio/ts/buffer.hpp"
#include "asio/ts/internet.hpp"*/
#include "nlohmann/json.hpp"

class PkgManager {
public:
	PkgManager();
	~PkgManager();

	void SetFetchURL(const std::string& url);
	void Refresh();

	int GetPkgCount() const;
	const std::string& GetEntryName(int id);
	const std::string& GetEntryAuthor(int id);
	int GetEntryID(int i);
	void DownloadEntry(int id);
	
	bool IsListReady();

private:
	nlohmann::json m_Entries;
	std::string m_URL;
	bool m_bReady;
};

#endif
