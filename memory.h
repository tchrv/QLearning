#pragma once

#include <vector>
#include <tuple>

#include "experience.h"

template <size_t s>
using MemoryRecord = std::tuple<ExperienceState<s>, ExperienceAction, DQLA_FP_TYPE, ExperienceState<s> >;



template <size_t in>
class MemorySystem {
public:
	virtual void save(MemoryRecord<in>& r) = 0;
	virtual std::vector<MemoryRecord<in> > read_minibatch(size_t size, DQLA_FP_TYPE seed) = 0;
	virtual size_t count() = 0;
};
