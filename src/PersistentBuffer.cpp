#include "PersistentBuffer.hpp"
#include "cassert"
#include "Output.h"

PersistentBuffer::PersistentBuffer() {
	m_NumRegions = 0;
	m_RegionIndex = 0;
	m_RegionSize = 0;
	m_pDrawFence = nullptr;
	m_Buffer = 0;
	m_pMemory = nullptr;
}

PersistentBuffer::~PersistentBuffer() {
	Cleanup();
}

bool PersistentBuffer::Init(int size, int num_regions) {
	assert(num_regions > 0);

	Cleanup();

	// Create a persistent buffer
	buffer_descriptor_t desc = { (GLsizei)size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT };
	CreateStaticBuffer(desc, &m_Buffer);
	void* data = (void*)glMapNamedBufferRange(m_Buffer, 0, size * num_regions, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	// Sync objects
	m_pDrawFence = new GLsync[num_regions];
	m_pDrawFence[0] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	
	// Region related
	m_RegionIndex = 0;
	m_NumRegions = num_regions;
	m_RegionSize = size;

	// Memory
	m_pMemory = data;
	return true;
}

void* PersistentBuffer::RequestBuffer() {
	assert(nullptr != m_pDrawFence);
	assert(nullptr != m_pMemory);
	int i = m_RegionIndex;
	int current = m_RegionIndex;
	GLenum e;

	while(1) {
		GLenum signal = glClientWaitSync(m_pDrawFence[i], GL_SYNC_FLUSH_COMMANDS_BIT, 100);
		if(GL_WAIT_FAILED == signal) {
			GL_ERROR();
			return nullptr;
		}

		if(GL_ALREADY_SIGNALED == signal || GL_CONDITION_SATISFIED == signal) {
			glDeleteSync(m_pDrawFence[i]);
			m_RegionIndex = i + 1;
			return ((char*)m_pMemory) + (m_RegionSize * i); 
		}
		i++;
		if (i >= m_NumRegions) {
			i = 0;
		}
		if (i == m_RegionIndex) {
			return nullptr;
		}
	}
}

void PersistentBuffer::Cleanup() {
	if(nullptr != m_pDrawFence) {
		for(int i = 0; i < m_NumRegions; i++) {
			glDeleteSync(m_pDrawFence[i]);
		}
		delete[] m_pDrawFence;
		m_pDrawFence = nullptr;
	}
	if(nullptr != m_pMemory) {
		glDeleteBuffers(1, &m_Buffer);
		m_pMemory = nullptr;
	}
}

GLuint PersistentBuffer::GetBufferID() const {
	return m_Buffer;
}
