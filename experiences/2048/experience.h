#pragma once

#include "../../experience.h"

class Experience2048 : public Experience<16, 4> {
public:
	Experience2048();

	std::array<ExperienceAction, 4> available_actions();
	ExperienceState<16> get_initial_state();
	bool is_terminal_state(ExperienceState<16>&);
	std::pair<DQLA_FP_TYPE, ExperienceState<16> > get_transition(ExperienceAction&, ExperienceState<16>&s);
	void display_nice(ExperienceState<16>&);
	void evaluate(QBrain<16, 4>&);
};
