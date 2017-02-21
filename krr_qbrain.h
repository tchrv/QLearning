#pragma once

#include "qbrain.h"
#include <dlib/dnn.h>

#include <cstdlib>

template <typename net_type, size_t in, size_t out>
class DeepQBrain : public QBrain<in, out> {
public:
	
	DeepQBrain();
	~DeepQBrain();

	void train_one_step(ExperienceState<in>&, std::array<DQLA_FP_TYPE, out>&);
	void train_batch(std::vector<ExperienceState<in> >&, std::vector<std::array<DQLA_FP_TYPE, out> >&); 

	std::array<DQLA_FP_TYPE, out> get_scores(ExperienceState<in>&);
	std::vector <std::array<DQLA_FP_TYPE, out> > get_all_scores(std::vector<ExperienceState<in> >& states);
private:
	dlib::dnn_trainer<net_type>* m_trainer;
	net_type m_net;
};


#include "deep_qbrain_impl.hpp"

