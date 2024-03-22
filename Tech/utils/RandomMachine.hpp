#pragma once

#ifndef INCLUDE_UTILS_RANDOMMACHINE_H_
#define INCLUDE_UTILS_RANDOMMACHINE_H_

#include "StandardLibs.hpp"

namespace tech
{
	template <class T>
	class random_machine {
	private:
		std::mt19937 m_engine;
	public:
		random_machine() {
			auto r = std::random_device();
			std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
			m_engine = std::mt19937(seed);
		}

		T get(T min, T max) {
			std::uniform_int_distribution<T> uniform_dist(min, max);
			return uniform_dist(m_engine);
		};
	};

}

#endif


