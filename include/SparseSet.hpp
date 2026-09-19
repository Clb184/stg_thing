#ifndef SPARSESET_INCLUDED
#define SPARSESET_INCLUDED

#include "Entity.hpp"
#include "string.h"

template<typename T, int max_ents>
class FixedSparseDataSet {
public:
	FixedSparseDataSet() {
		m_DenseCnt = 0;
		memset(m_DenseData, 0, sizeof(m_DenseData));
		memset(m_SparseID, 0xffffffff, sizeof(m_SparseID));
	}

	~FixedSparseDataSet() {

	}

	void Init() {
		Clear();
	}

	int Add(const T& data, Entity id) {
		id >>= 8;
		assert(id < max_ents && id >= 0);
		int idx = m_DenseCnt;
		m_DenseCnt++;
		m_DenseData[idx] = data;
		m_SparseID[id] = idx;
		return idx;
	}

	void Delete(Entity id) {
		id >>= 8;
		assert(id < max_ents && id >= 0);

		int idx = m_SparseID[id];
		if(idx != 0xffffffff) {
			// First, get the last element and its entity id
			m_DenseCnt--;
			int last_idx = m_DenseCnt;
			int ent_last_id = m_DenseToEntity[last_idx];

			// Then, replace the one removed value with the last one
			m_DenseData[idx] = m_DenseData[last_idx];

			// Replace the value index corresponding to that entity
			m_SparseID[ent_last_id] = idx;
			m_DenseToEntity[idx] = ent_last_id;

			m_SparseID[id] = 0xffffffff;
		}
	}
	
	T* Get(Entity id) {
		id >>= 8;
		assert(id < max_ents && id >= 0);
		int idx = m_SparseID[id];
		assert(idx != 0xffffffff);
		return &m_DenseData[idx];
	}

	void Set(Entity id, const T& val) {
		id >>= 8;
		assert(id < max_ents && id >= 0);
		int idx = m_SparseID[id];
		assert(idx != 0xffffffff);
		m_DenseData[idx] = val;
	}
	
	void Clear() {
		m_DenseCnt = 0;
		memset(m_SparseID, 0xffffffff, sizeof(m_SparseID));
	}

	int Size() const {
		return m_DenseCnt;
	}

private:
	int m_DenseCnt;
	T m_DenseData[max_ents];
	Entity m_DenseToEntity[max_ents];
	int m_SparseID[max_ents];
};

template<int max_ents>
class FixedSparseSet {
public:
	FixedSparseSet() {
		m_DenseCnt = 0;
		memset(m_SparseID, 0xffffffff, sizeof(m_SparseID));
	}

	~FixedSparseSet() {

	}

	void Init() {
		Clear();
	}

	int Add(Entity id) {
		id >>= 8;
		assert(id < max_ents && id >= 0);
		int idx = m_DenseCnt;
		m_DenseCnt++;
		m_SparseID[id] = idx;
		return idx;
	}

	void Delete(Entity id) {
		id >>= 8;
		assert(id < max_ents && id >= 0);

		int idx = m_SparseID[id];
		if(idx != 0xffffffff) {
			// First, get the last element and its entity id
			m_DenseCnt--;
			int last_idx = m_DenseCnt;
			int ent_last_id = m_DenseToEntity[last_idx];

			// Replace the value index corresponding to that entity
			m_SparseID[ent_last_id] = idx;
			m_DenseToEntity[idx] = ent_last_id;

			m_SparseID[id] = 0xffffffff;
		}
	}
	
	int Get(Entity id) {
		id >>= 8;
		assert(id < max_ents && id >= 0);
		int idx = m_SparseID[id];
		assert(idx != 0xffffffff);
		return idx;
	}

	void Clear() {
		m_DenseCnt = 0;
		memset(m_SparseID, 0xffffffff, sizeof(m_SparseID));
	}

	int Size() const {
		return m_DenseCnt;
	}

private:
	int m_DenseCnt;
	Entity m_DenseToEntity[max_ents];
	int m_SparseID[max_ents];
};

#endif
