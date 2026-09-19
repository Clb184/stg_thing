#ifndef ENTITYMANAGER_INCLUDED
#define ENTITYMANAGER_INCLUDED 

#include "Entity.hpp"
#include "vector"

template<int ent_cnt>
class EntityManager {
public:
	EntityManager() {
		m_Size = 0;
		m_IDs = 0;
		m_FreeList.reserve(ent_cnt);
	}

	~EntityManager() {

	}

	void Init() {
		Reset();
	}

	Entity CreateEntity() {
		if (m_Size >= ent_cnt) return 0xffffffff;

		if(false != m_FreeList.empty()) {
			Entity ent = m_FreeList.back();
			ent = (ent & 0xffffff00) | ((ent + 1) & 0xff); // 8 bits for gen, wraps to 0 at 256, up generation with that cap
			m_FreeList.pop_back();
			m_Size++;
			return ent;
		}

		Entity id = m_IDs << 8;
		m_IDs++;
		return id;
	}

	void DeleteEntity(Entity e) {
		if(m_Size <= 0) return;

		m_FreeList.push_back(e);
		m_Size--;
	}
	
	bool Full() const {
		return m_Size >= ent_cnt;
	}
	
	void Reset() {
		m_Size = 0;
		m_IDs = 0;
		m_FreeList.clear();
	}

private:
	int m_Size;
	uint32_t m_IDs;
	std::vector<Entity> m_FreeList;
//	uint16_t m_Entities[ent_cnt]; // high 8 is gen, low 8 is if enabled or not // Max of 255 gens
};

#endif
