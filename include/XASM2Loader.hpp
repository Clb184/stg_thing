#ifndef XASM2LOADER_INCLUDED
#define XASM2LOADER_INCLUDED

#include "PackArchive/PackFile.h"
#include "cstdint"

class XASM2Loader {
public:
	XASM2Loader();
	~XASM2Loader();

	bool Load(const char* script);
	bool LoadP(pack_file_t* pack, const char* entry);
	bool LoadEx(char* data, size_t size);

	uint8_t* GetBase();
	int GetEntryPoint() const;
	void Cleanup();

private:
	bool Check(uint8_t* data, size_t size);

private:
	uint8_t* m_pData;
	int m_Entry;
};

#endif
