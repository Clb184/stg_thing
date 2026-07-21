#include "PkgManager.hpp"
#include "Output.h"
#include "IO.h"
#include "filesystem"
#include "string"

#ifdef WIN32
#include "windows.h"
#include "wininet.h"

#pragma comment(lib, "wininet.lib")
#endif



PkgManager::PkgManager() {
	std::filesystem::create_directory(".brt_cache");
	std::filesystem::create_directory("pkgs");
}

PkgManager::~PkgManager() {
	WriteDownloadedCache();
	WritePackageCache();
}

bool PkgManager::Init() {
	LoadDownloadedCache();
	LoadPackageCache();
	return true;
}

void PkgManager::SetFetchURL(const std::string& url) {
	char buf[512];
	sprintf(buf, "Fetch URL: %s", url.c_str());
	LOG_INFO(buf);
	m_URL = url;
}

size_t WriteData(void* data, size_t size, size_t cnt, FILE* fp) {
	return fwrite(data, size, cnt, fp);
}

#ifdef WIN32
bool DownloadWindows(const char* url, const char* out) {
	char buf[512];
	sprintf(buf, "Downloading from: %s > %s", url, out);
	LOG_INFO("Downloading file (Windows)");
	HINTERNET inet = InternetOpen("WinInetDownloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!inet) {
		LOG_ERROR("Failed connecting to the Internet");
	       	return false;
	}

	HINTERNET hurl = InternetOpenUrl(inet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if(!hurl) {
		LOG_ERROR("Failed opening URL");
		InternetCloseHandle(inet);
		return false;
	}
	
	char* buffer = (char*)calloc(1, 4096);
	FILE* fp = fopen(out, "wb");
	DWORD readen = 0;
	DWORD sum = 0;
	LOG_INFO("Begin download");
	while (InternetReadFile(hurl, buffer, 4096, &readen) && readen > 0) {
		fwrite(buffer, readen, 1, fp);
	}
	free(buffer);
	LOG_INFO("End download");

	fclose(fp);
	InternetCloseHandle(hurl);
	InternetCloseHandle(inet);
	return true;
}

#endif

bool PkgManager::Refresh() {
	LOG_INFO("Refreshing list of available packages");
	httplib::Client client(m_URL);
		
	if(auto res = client.Get("/api/packages")) {
		if(200 == res->status) {
			std::cout << "OK, got packages:" << res->body << "\n";
			m_Entries = nlohmann::json::parse(res->body);
		}
	} else {
		LOG_ERROR("Couldn't comunicate with the server");
		return false;
	}
	WritePackageCache();
	return true;
}

int PkgManager::GetPkgCount() const {
	int sz = 0;
	try {
		sz = m_Entries["entries"].size();
	} catch(...) { }
	return sz;
}

std::string PkgManager::GetEntryName(int id) {
	std::string ret = "";
	try {
		ret = m_Entries["entries"][id]["name"];
	} catch(...) { }
	return ret;
}

std::string PkgManager::GetEntryAuthor(int id) {
	std::string ret = "";
	try {
		ret = m_Entries["entries"][id]["author"];
	} catch(...) { }
	return ret;
}

std::string PkgManager::GetPkgDir(int id) {
	std::string ret = "";
	try {
		int pkg_id = m_Entries["entries"][id]["id"];
		ret = m_Downloaded[std::to_string(pkg_id)]["path"];
	} catch(...) { }
	return ret;
}

bool PkgManager::GetAvailable(int id) {
	bool ret = false;
	try {
		int pkg_id = m_Entries["entries"][id]["id"];
		ret = m_Downloaded.find(std::to_string(pkg_id)) != m_Downloaded.end();
	} catch(...) { }
	return ret;
}

int PkgManager::GetEntryID(int id) {
	int ret = -1;
	try {
		ret = m_Entries["entries"][id]["id"];
	} catch(...) { }
	return ret;
}

void PkgManager::DownloadEntry(int id) {
	std::string url = "";
	try {
		url = m_Entries["entries"][id]["url"];
	} catch(...) {
		LOG_ERROR("Failed getting URL from entry");
		return;
	}

	std::string out = "";
	std::cout << "Downloading from: " << url << "\n";
	int i = url.length() - 1;
	while(i >= 0 && (url.at(i) != '/' && url.at(i) != '\\')) {
		i--;
	}

	std::cout << "I is: " << i << "\n";

	if(i< 0) {
		i = 0;
	} 
	else if(url.at(i) == '/' || url.at(i) == '\\') {
		i++;
	}
	
	for(int c = i; c < url.length(); c++) {
		out.push_back(url.at(c));
	}
	std::cout << "Output name: " << out << "\n";
	
	out = "pkgs/" + out;
	
	int entry_id = m_Entries["entries"][id]["id"];
	bool result = false;
#ifdef WIN32
	result = DownloadWindows(url.c_str(), out.c_str());
#endif
	if(result) {
		m_Downloaded[std::to_string(entry_id)] = nlohmann::json::parse("{\"path\" : \"" + out + "\" }");
		WriteDownloadedCache();
	}
}

bool PkgManager::IsListReady() {
	return true;
}

void PkgManager::Cleanup() {
	WriteDownloadedCache();
	WritePackageCache();
}

void PkgManager::SaveCache() {
	WriteDownloadedCache();
	WritePackageCache();
}

void PkgManager::LoadDownloadedCache() {
	char* data;
	size_t size;
	if(true == LoadTextFromFile(".brt_cache/downloaded.json", &data, &size)) {
		m_Downloaded = nlohmann::json::parse(data);
		free(data);
	} else {
		m_Downloaded = nlohmann::json::parse("{}");
	}
}

void PkgManager::LoadPackageCache() {
	char* data;
	size_t size;
	if(true == LoadTextFromFile(".brt_cache/packages.json", &data, &size)) {
		m_Entries = nlohmann::json::parse(data);
		free(data);
	} else {
		m_Entries = nlohmann::json::parse("{ \"entries\" : [] }");
	}
}

void PkgManager::WriteDownloadedCache() {
	FILE* fp = fopen(".brt_cache/downloaded.json", "w");
	std::string str = m_Downloaded.dump(4);
	if(nullptr != fp) {
		fwrite(str.data(), str.length(), 1, fp);
		fclose(fp);
	}
}

void PkgManager::WritePackageCache() {
	FILE* fp = fopen(".brt_cache/packages.json", "w");
	std::string str = m_Entries.dump(4);
	if(nullptr != fp) {
		fwrite(str.data(), str.length(), 1, fp);
		fclose(fp);
	}
}
