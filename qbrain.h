#pragma once

#include <cstdlib>
#include <vector>
#include <array>
template <size_t s>
using ExperienceState = std::array<DQLA_FP_TYPE, s>;

using ExperienceAction = int;

template <size_t s>
bool state_changed(const ExperienceState<s> old, const ExperienceState<s> n) {
	for(int i = 0; i < s; i++)
		if(old[i] != n[i]) return true;
	return false;
}

template <size_t state_size, size_t num_actions>
class QBrain {
public:
	virtual void train_one_step(ExperienceState<state_size>&, std::array<DQLA_FP_TYPE, num_actions>&) = 0;
	virtual void train_batch(std::vector<ExperienceState<state_size> >&, std::vector<std::array<DQLA_FP_TYPE, num_actions> >&) = 0;

	virtual std::array<DQLA_FP_TYPE, num_actions> get_scores(ExperienceState<state_size>&) = 0;
	virtual std::vector <std::array<DQLA_FP_TYPE, num_actions> > get_all_scores(std::vector<ExperienceState<state_size> >& states) = 0;
	virtual ~QBrain() {}
};
