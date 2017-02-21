#pragma once

#include "experience.h"
#include "qbrain.h"
#include "memory.h"

#include <cstdlib>

template <size_t in, size_t out>
class QLearningAlgorithm {
public:
	QLearningAlgorithm(Experience<in, out>&, QBrain<in, out>&, MemorySystem<in>& system);
	
	void run();
	void set_learning_rate(DQLA_FP_TYPE);
	void set_discount_factor(DQLA_FP_TYPE);
	void set_alt_search_prob(DQLA_FP_TYPE);
	void set_minibatch_size(size_t);
	void set_max_iterations(int);
	
	DQLA_FP_TYPE get_learning_rate();
	DQLA_FP_TYPE get_discount_factor();
	DQLA_FP_TYPE get_alt_search_prob();
	size_t get_minibatch_size();
	
	void verbose();
	void quiet();
	
private:
	int m_max_iterations;
	bool m_verbose;
	size_t m_minibatch_size;
	DQLA_FP_TYPE m_learning_rate;
	DQLA_FP_TYPE m_discount_factor;
	DQLA_FP_TYPE m_alt_search_prob;
	
	Experience<in, out>& m_experience;
	QBrain<in, out>& m_brain;
	MemorySystem<in>& m_mem;
};
#include "q_learning_algorithm_impl.hpp"
