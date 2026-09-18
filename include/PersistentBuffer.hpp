#ifndef PERSISTENTBUFFER_INCLUDED
#define PERSISTENTBUFFER_INCLUDED

#include "GL/glew.h"
#include "OpenGL/Buffer.h"
#include "cassert"
#include "Output.h"

template<int num_regions>
class PersistentBuffer {
public:
	PersistentBuffer(){
		m_RegionIndex = 0;
		m_RegionSize = 0;
		m_pDrawFence = nullptr;
		m_Buffer = 0;
		m_pMemory = nullptr;
	}

	~PersistentBuffer() {
		Cleanup();
	}

	bool Init(int size) {
		assert(num_regions > 0);
		GLERR;
	
		Cleanup();
	
		// Create a persistent buffer
		buffer_descriptor_t desc = { (GLsizei)(size * num_regions), nullptr, GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT };
		CreateStaticBuffer(desc, &m_Buffer);
		void* data = (void*)glMapNamedBufferRange(m_Buffer, 0, size * num_regions, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		GL_ERROR();
	
		// Sync objects
		m_pDrawFence = new GLsync[num_regions];
		m_pDrawFence[0] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		
		// Region related
		m_RegionIndex = 0;
		m_RegionSize = size;
	
		// Memory
		m_pMemory = data;
		return true;
	}

	void* RequestBuffer() {
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
			if (i >= num_regions) {
				i = 0;
			}
			if (i == m_RegionIndex) {
				return nullptr;
			}
		}
	}

	void Cleanup() {
		if(nullptr != m_pDrawFence) {
			for(int i = 0; i < num_regions; i++) {
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

	GLuint GetBufferID() const {
		return m_Buffer;
	}

private:

private:
	int m_RegionIndex;
	int m_RegionSize;

	GLsync* m_pDrawFence;
	GLuint m_Buffer;

	void* m_pMemory;
};

#endif
