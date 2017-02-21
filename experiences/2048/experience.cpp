#include "experience.h"

#include "grid.hpp"
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <array>

Experience2048::Experience2048() {}

inline ExperienceState<16> convert_game_state(grid const& game) {
	ExperienceState<16> s;
	for(int x = 0; x < 4; x++)
	for(int y = 0; y < 4; y++) {
		int v = game.get(x, y);
		s[4*y+x] = v == 0 ? 0 :std::log2(v);
	}
	return s;
}

inline grid convert_state_game(ExperienceState<16>& state) {
	grid game;
	for(int x = 0; x < 4; x++)
	for(int y = 0; y < 4; y++) {
		int v(state[y*4 + x]);
		game.set(x, y, v == 0 ? 0 : 1 << v); 
	}
	return game;
}


std::array<ExperienceAction, 4> Experience2048::available_actions() {
	return {0,1,2,3};
}


ExperienceState<16> Experience2048::get_initial_state() {
	grid game;
	return convert_game_state(game);
}


bool Experience2048::is_terminal_state(ExperienceState<16>& s) {
	return !convert_state_game(s).can_move();
}

float tile_diff_score(float t1, float t2) {
	return 1.0/(0.1 + std::abs(t1 - t2));
}

float calc_state_score(ExperienceState<16> s) {
	float r = 0;

	//Smoothness evalutation
	float tile_score[16];
	for(int i = 0; i < 16; i++)
		tile_score[i] = 0;
	for(int x = 0; x < 4; x++)
	for(int y = 0; y < 4; y++)
	{
			int c = 0;
			float v = s[4*y+x];
			if(y > 0) {
				tile_score[4*y + x] += tile_diff_score(v, s[4*(y-1)+x]);
				c++;
			}
			if(x > 0) {
				tile_score[4*y + x] += tile_diff_score(v, s[4*y+x-1]);
				c++;
			}
			if(y < 3) {
				tile_score[4*y + x] += tile_diff_score(v, s[4*(y+1)+x]);
				c++;
			}
			if(x < 3) {
				tile_score[4*y + x] += tile_diff_score(v, s[4*y+x+1]);
				c++;
			}
			tile_score[4*y + x] *= v/c;
			/*float v1 = s[4*y + x], v2 = s[4*(y+1) + x], v3 = s[4*y + x + 1];
			if(v1 == v2) 
				tile_score[4*y + x] = tile_score[4*(y+1) + x] = v1+1;
			if(v1 == v3) 
				tile_score[4*y + x] = tile_score[4*y + x + 1] = v1+1;*/
			
		/*} else if(x == 3 && y < 3) {
			float v1 = s[4*y + x], v2 = s[4*(y+1) + x];
			if(v1 == v2) 
				tile_score[4*y + x] = tile_score[4*(y+1) + x] = v1+1;
		} else if(y == 3 && x < 3) {
			float v1 = s[4*y + x], v3 = s[4*y + x + 1];
			if(v1 == v3) 
				tile_score[4*y + x] = tile_score[4*y + x + 1] = v1+1;
		}*/
	}
	
	
	// Higher values in corners
	int maxx = 0, maxy = 0;
	float max_corner = s[0];
	if(s[3] > max_corner) { maxx=3; maxy=0;max_corner=s[3];}
	if(s[11] > max_corner) { maxx=0; maxy=3;max_corner=s[11];}
	if(s[15] > max_corner) { maxx=3; maxy=3;max_corner=s[15];}
	
	float mask[16];
	for(int x=0; x < 4; x++)
	for(int y=0; y < 4; y++) {
		mask[4*y+x] = pow(10, (6-(std::abs(x-maxx) + std::abs(y-maxy)))*0.05*s[4*maxy + maxx]);
	}
	/*const float mask[] = {
		100.0, 40.0, 40.0, 100.0,
		40.0, 1.0, 1.0, 40.0,
		40.0, 1.0, 1.0, 40.0,
		100.0, 40.0, 40.0, 100.0
	};*/
	
	
	
	for(int i = 0; i < 16; i++) {
		r += mask[i] * (tile_score[i] + s[i]);
	}


	//Free tiles evaluation
	for(int i = 0; i < 16; i++)
		if(s[i] == 0) r += 50;

	return 10*r;

}

std::pair<float, ExperienceState<16> > Experience2048::get_transition(ExperienceAction& action, ExperienceState<16>& curr_state) {
	
	std::pair<float, ExperienceState<16> > transition;
	float reward = 0;
	grid g = convert_state_game(curr_state);

	
	switch(action) {
		case 0:
			g.action(direction::NORTH, false);
			break;
		case 1:
			g.action(direction::EAST, false);
			break;
		case 2:
			g.action(direction::SOUTH, false);
			break;
		case 3:
			g.action(direction::WEST, false);
			break;
	}
	ExperienceState<16> s2 = convert_game_state(g);
	bool changed = false;
	for(int i = 0; i < 16 && !changed; i++)
		if(curr_state[i] != s2[i]) changed = true;
	if(changed) {
		int prev_score = calc_state_score(curr_state);// + 2 * g.score();
		int new_score = calc_state_score(convert_game_state(g))+ 2*g.score();
		transition.first = new_score - prev_score;
	} else {
		transition.first = -1e10;
	}
	g = convert_state_game(curr_state);
	switch(action) {
		case 0:
			g.action(direction::NORTH);
			break;
		case 1:
			g.action(direction::EAST);
			break;
		case 2:
			g.action(direction::SOUTH);
			break;
		case 3:
			g.action(direction::WEST);
			break;
	}
	transition.second = convert_game_state(g);
	//transition.first = calc_state_score(transition.second) - calc_state_score(curr_state);//(new_score - prev_score + 5*(prev_occupied - new_occupied) + g.score()/5);

	
	return transition;
}


void Experience2048::evaluate(QBrain<16, 4>& brain) {
	ExperienceState<16> game_state;
	const unsigned int num_exp = 50;
	float mean = 0;
	float mean_score = 0;
	unsigned int vmax = 0, smax = 0;
	for(int t = 0 ; t < num_exp; t++) { 
		grid g;
		bool changed = true;	
		game_state = convert_game_state(g);
		while(!is_terminal_state(game_state) && changed) {
			std::array<DQLA_FP_TYPE, 4> q_scores = brain.get_scores(game_state);
			if(t == 0) {
				for(int i = 0; i < 4; i++) {
					std::cout << "Action " << i << q_scores[i] << std::endl;
				}
			}
			int action = std::distance(q_scores.begin(), std::max_element(q_scores.begin(), q_scores.end()));
			switch(action) {
		         	case 0:
        	              		g.action(direction::NORTH);
                	        	break;
                		case 1:
                       		 	g.action(direction::EAST);
                        		break;
                		case 2:
                        		g.action(direction::SOUTH);
                        		break;
               			case 3:
	                	        g.action(direction::WEST);
        	        	        break;
        		}
		
			ExperienceState<16> s2 = convert_game_state(g);
			changed = state_changed(game_state, s2);
			game_state = convert_game_state(g);
		}
		unsigned int v = g.largest(), s = g.score();
		vmax = std::max(vmax, v);
		smax = std::max(smax, s);
		mean += v;
		mean_score += s;
	}
	std::cout << "Valeur maximum moyenne: " << mean/num_exp << ", score moyen: " << mean_score/num_exp << ". Valeur maximum atteinte: " << vmax << ", score max:" << smax<< std::endl;
}

void Experience2048::display_nice(ExperienceState<16>& s) {
	convert_state_game(s).print();
}
