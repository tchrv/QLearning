#pragma once

#include <random>
#include <algorithm>
#include <iostream>

template <size_t in, size_t out>
QLearningAlgorithm<in ,out>::QLearningAlgorithm(Experience<in, out>& exp, QBrain<in, out>& brain, MemorySystem<in>& sys) : m_experience(exp), m_brain(brain), m_mem(sys),
											        m_verbose(false),
												m_learning_rate(0.9), m_discount_factor(0.8), m_alt_search_prob(0.1), 
												m_max_iterations(1000), m_minibatch_size(100)
{}

template <typename T, size_t s>
int index_of_max(std::array<T, s> v ) {
	auto b = v.begin();
	return std::distance(b, std::max_element(b, v.end()));

}

template<typename T, size_t s>
int index_of_action(std::array<T,s> v) {

}

const float temperature = 15.0;

template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::run() {
	ExperienceState<in> currState, nextState, batchState;
	std::default_random_engine generator;
  	std::uniform_real_distribution<DQLA_FP_TYPE> randgen(0.0,1.0);

	std::vector<DQLA_FP_TYPE> actions_scores;
	std::array<ExperienceAction, out> available_actions = m_experience.available_actions();
	std::uniform_int_distribution<int> randintgen(0, out-1);
	ExperienceAction action;
	float rwd;
	
	

	currState = m_experience.get_initial_state();
	int i = 0;
	float alt_search_prob = m_alt_search_prob;
	int iter_unchanged = 0;
	while(i++ < m_max_iterations) {
		if(m_experience.is_terminal_state(currState))
			currState = m_experience.get_initial_state();
		if(randgen(generator) < alt_search_prob) {
			action = randintgen(generator);
//			if(m_verbose) std::cout << "Picked random action " << action << std::endl;
		} else {
			action = index_of_max(m_brain.get_scores(currState));
		}
		auto trans = m_experience.get_transition(action, currState);
		
		rwd = trans.first;
		nextState = trans.second;
		bool changed = state_changed(currState, nextState);

		if(changed) {
			MemoryRecord<in> record(currState, action, rwd, nextState);
			m_mem.save(record);
			alt_search_prob = m_alt_search_prob;
			iter_unchanged = 0;
		} else {
			alt_search_prob *= 1.04;
			if(++iter_unchanged > 50 && m_verbose) {
				std::cout << "Found problematic state! :" << std::endl;
				m_experience.display_nice(currState);
			}
		}
		if(m_mem.count() > m_minibatch_size) {
			std::vector<MemoryRecord<in>> records = m_mem.read_minibatch(m_minibatch_size, randgen(generator));
			std::vector<ExperienceState<in> > batch_states(records.size());
			std::vector<std::array<float, out> > batch_actions_scores(records.size());
			for(MemoryRecord<in> r : records) {
				batchState = std::get<0>(r);
				std::array<float, out> current_actions_score = m_brain.get_scores(batchState);
				float target, current_score = std::get<2>(r);
			
				if(m_experience.is_terminal_state(batchState)) {
					target = current_score;
				} else {
					auto actions_scores2 = m_brain.get_scores(std::get<3>(r));
					target = current_score + m_learning_rate * (m_discount_factor * actions_scores2[index_of_max(actions_scores2)] - current_score);
				}
				current_actions_score[std::get<1>(r)] = target;
				batch_states.push_back(batchState);
				batch_actions_scores.push_back(current_actions_score);
			}

			m_brain.train_batch(batch_states, batch_actions_scores);
		}
		if(m_verbose && changed && m_mem.count() % 500 == 0) {
			std::cout << "Nombre d'etats en memoire: " << m_mem.count() << std::endl;
		 	m_experience.evaluate(m_brain);
		}
		currState = nextState;
				
	}
}
template <size_t in, size_t out>
DQLA_FP_TYPE QLearningAlgorithm<in, out>::get_learning_rate() { return m_learning_rate; }
template <size_t in, size_t out>
DQLA_FP_TYPE QLearningAlgorithm<in, out>::get_discount_factor() { return m_discount_factor; }
template <size_t in, size_t out>
DQLA_FP_TYPE QLearningAlgorithm<in, out>::get_alt_search_prob() { return m_alt_search_prob; }
template <size_t in, size_t out>
size_t QLearningAlgorithm<in, out>::get_minibatch_size() { return m_minibatch_size; }

template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::set_learning_rate(DQLA_FP_TYPE lr) { m_learning_rate = lr; }
template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::set_discount_factor(DQLA_FP_TYPE df) { m_discount_factor = df; }
template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::set_alt_search_prob(DQLA_FP_TYPE prob) { m_alt_search_prob = prob; }
template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::set_minibatch_size(size_t s) { m_minibatch_size = s; }
template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::set_max_iterations(int iter) { m_max_iterations = iter; }

template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::verbose() { m_verbose = true; }
template <size_t in, size_t out>
void QLearningAlgorithm<in, out>::quiet() { m_verbose = false; }

