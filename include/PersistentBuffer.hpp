#ifndef PERSISTENTBUFFER_INCLUDED
#define PERSISTENTBUFFER_INCLUDED

#include "GL/glew.h"
#include "OpenGL/Buffer.h"

class PersistentBuffer {
public:
	PersistentBuffer();
	~PersistentBuffer();

	bool Init(int size, int num_regions);
	void* RequestBuffer();
	void Cleanup();
	GLuint GetBufferID() const;

private:

private:
	int m_NumRegions;
	int m_RegionIndex;
	int m_RegionSize;

	GLsync* m_pDrawFence;
	GLuint m_Buffer;

	void* m_pMemory;
};

#endif
