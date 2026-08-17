#include "XASM2Loader.hpp"
#include "IO.h"
#include "Output.h"
#include "cassert"

XASM2Loader::XASM2Loader() {

}

XASM2Loader::~XASM2Loader() {

}

bool XASM2Loader::Load(const char* script) {
	LOG_INFO("Loading XASM2 script from file...");
	uint8_t* data = nullptr;
	size_t size = 0;
	if(LoadDataFromFile(script, (void**)&data, &size)) {
		if(!Check(data, size)) return false;
	}
	else {
		LOG_ERROR("File not found");
		return false;
	}
	return true;
}

bool XASM2Loader::LoadP(pack_file_t* pack, const char* entry) {
	LOG_INFO("Loading XASM2 script from pack file...");
	uint8_t* data = nullptr;
	size_t size = 0;

	if(0 == PackFileLoadEntry(pack, entry, (void**)&data, &size)) {
		if(!Check(data, size)) return false;
	} else {
		LOG_ERROR("Entry not found");
		return false;
	}
	
	return true;
}

bool XASM2Loader::LoadEx(char* data, size_t size) {
	LOG_INFO("Loading XASM2 script from memory");
	if(!Check((uint8_t*)data, size)) return false;
	return true;
}

uint8_t* XASM2Loader::GetBase() {
	return m_pData;
}

int XASM2Loader::GetEntryPoint() const {
	return m_Entry;
}

void XASM2Loader::Cleanup() {
	if(0 != m_pData) {
		free(m_pData);
		m_pData = 0;
	}
	m_Entry = 0;
}

bool XASM2Loader::Check(uint8_t* data, size_t size) {
	bool valid = false;
	// Check is at least magic + entry size
	if(size > (8 + 4)) {
		// Check XASM2 magic
		bool sign =
			data[0] == 'X' &&
			data[1] == 'A' &&
			data[2] == 'S' &&
			data[3] == 'M' &&
			data[4] == '0' &&
			data[5] == '0' &&
			data[6] == '0' &&
			data[7] == '2';
		if(sign) {
			int entry = *(int*)(data + 8);
			printf("EntryPoint: %X\n", entry);

			// Till here I check these
			if(entry < size) {
				m_pData = data;
				m_Entry = entry;
				valid = true;
			}
			else {
				LOG_ERROR("Invalid EntryPoint offset");
			}
		}
		else {
			LOG_ERROR("Invalid script magic");
		}
	}

	if(valid) {
		LOG_INFO("Loaded XASM2 script");
	}
	else {
		LOG_INFO("Invalid XASM2 script");
	}
	assert(true == valid);
	return valid;
}

