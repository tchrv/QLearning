#pragma once

#include "memory.h"
#include <random>
#include <ctime>

template <size_t in>
class RamMemorySystem : public MemorySystem<in>{
public:
	RamMemorySystem() {}
	void save(MemoryRecord<in>& record) { m_vec.push_back(record); }
	std::vector<MemoryRecord<in> > read_minibatch(size_t minibatch_size, DQLA_FP_TYPE seed) {
		std::default_random_engine generator(seed);
  		std::uniform_int_distribution<int> randgen(0, m_vec.size()-1);

		std::vector<MemoryRecord<in> > out;

		for(int i = 0; i < minibatch_size; i++) {
			out.push_back(m_vec[randgen(generator)]);
		}
		return out;
	}
        size_t count() { return m_vec.size(); }
private:
	std::vector<MemoryRecord<in> > m_vec;
};
