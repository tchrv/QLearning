#include <stdio.h>
#include <vector>
#include <iostream>
#include "deep_qbrain.h"
#include "q_learning_algorithm.h"
#include "ram_memory.h"
#include "experiences/2048/experience.h"

#include <dlib/dnn.h>

using namespace dlib;

using in_layer = softmax<input< matrix<float, 16, 1> >>;
using hidden_layer = dropout< sig<fc< 1000, in_layer> > >;
using output_layer = loss_mean_squared< fc < 4, hidden_layer> >;

using net_type = output_layer;


using input_type = matrix<float>;
using output_type = matrix<float>;

int main() {

	net_type net;
	

	DeepQBrain<net_type, 16, 4> brain(net);

	Experience2048 exp;
	RamMemorySystem<16> ram;
	
	QLearningAlgorithm<16, 4> algo(exp, brain, ram);
	algo.set_max_iterations(5000000);
	algo.verbose();
	algo.run();
	
	return 0;
}
