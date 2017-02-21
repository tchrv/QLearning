#include "experience.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>



const std::string actions_names[4] = {"up", "right", "down", "left"};

int main() {
	Experience2048 exp;
	ExperienceState<16> s = exp.get_initial_state();
	exp.display_nice(s);
	while(!exp.is_terminal_state(s)) {
		
		std::array<float, 4> rewards;
		std::cout << std::endl;
		int a;
		float maxr = -1e15;
		for(int i = 0; i < 4; i++) {
			rewards[i] = exp.get_transition(i, s).first;
			if(rewards[i] > maxr) {
				a = i;
				maxr = rewards[i];
			}
		}
		for(int i = 0; i < 4; i++)
			std::cout << "Reward for action " << actions_names[i] << ": " << rewards[i] << (a == i ? " <---" : "") << std::endl;
		char c = getchar();
		if(c != '\n') {
			getchar(); 
			switch(c) {
				case 'u':
					a = 0;
					break;
				case 'r':
					a = 1;
					break;
				case 'd':
					a = 2;
					break;
				case 'l':
					a = 3;
					break;
			}
		}
		s = exp.get_transition(a, s).second;
		exp.display_nice(s);
	}
}
