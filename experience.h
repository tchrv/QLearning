#pragma once

#include <cstdlib>
#include <array>
#include <utility>

#include "qbrain.h"

template <size_t state_size, size_t num_actions>
class Experience {
public:
	virtual std::array<ExperienceAction, num_actions> available_actions() = 0;

	virtual ExperienceState<state_size> get_initial_state() = 0;

	virtual bool is_terminal_state(ExperienceState<state_size>&) = 0;

	virtual std::pair<DQLA_FP_TYPE, ExperienceState<state_size> > get_transition(ExperienceAction&, ExperienceState<state_size>&) = 0;
	
	virtual void display_nice(ExperienceState<state_size>&) {};
		
	virtual void evaluate(QBrain<state_size, num_actions>& brain) {};
};
