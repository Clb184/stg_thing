#include "PkgManager.hpp"
#include "Output.h"
#include "filesystem"

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
void DownloadWindows(const char* url, const char* out) {
	char buf[512];
	sprintf(buf, "Downloading from: %s > %s", url, out);
	LOG_INFO("Downloading file (Windows)");
	HINTERNET inet = InternetOpen("WinInetDownloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!inet) return;

	HINTERNET hurl = InternetOpenUrl(inet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if(!hurl) {
		LOG_ERROR("Failed opening URL");
		InternetCloseHandle(inet);
		return;
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
}

#endif

void PkgManager::Refresh() {
	LOG_INFO("Refreshing list of available packages");
	httplib::Client client(m_URL);
		
	if(auto res = client.Get("/api/packages")) {
		if(200 == res->status) {
			std::cout << "OK, got packages:" << res->body << "\n";
			m_Entries = nlohmann::json::parse(res->body);
		}
	} else {
		LOG_ERROR("Couldn't comunicate with the server");
	}
	
	//DownloadWindows("https://github.com/Clb184/upload_tests/raw/refs/heads/master/res.zip", "pkgs/res.zip");

}

int PkgManager::GetPkgCount() const {
	return m_Entries["entries"].size();
}

const std::string& PkgManager::GetEntryName(int id) {
	return m_Entries["entries"][id]["name"];
}

const std::string& PkgManager::GetEntryAuthor(int id) {
	return m_Entries["entries"][id]["author"];
}

int PkgManager::GetEntryID(int id) {
	return 0;
}

void PkgManager::DownloadEntry(int id) {
	std::string url = m_Entries["entries"][id]["url"];
	std::string out = "";
	int i = url.length() - 1;
	while((url.at(i) != '/' || url.at(i) != '\\') && i >= 0) {
		i--;
	}

	for(int c = i; c < url.length(); c++) {
		out.push_back(url.at(i));
	}
	std::cout << "Output name: " << out << "\n";
	
	out = "pkgs/" + out;

	DownloadWindows(url.c_str(), out.c_str());
}

bool PkgManager::IsListReady() {
	return true;
}
