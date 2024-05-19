#pragma once

#ifndef INCLUDE_UTILS_INTERVALSET_H_
#define INCLUDE_UTILS_INTERVALSET_H_

#include "IntervalMapx.hpp"

namespace tech
{
	/**
	 * interval_set<T> is a data structure that allows to store non-overlapping intervals of type T.
	 * The type T does not need to be default-constructible but must implement the comparison operator <.
	 * The class inherits from interval_mapx.
	 */
	template <typename K>
	class interval_set : public interval_mapx<K, bool> {

		using base = interval_mapx<K, bool>;

	public:

		interval_set(insert_mode mode) : base(mode, false) {}

		void insert(K const& start, K const& finish) {
			base::m_is_cache_valid = false;
			switch (base::m_mode)
			{
				case insert_mode::conservative: return base::insert_conservative_solid_intervals({ start }, { finish }, { true }, 1);
				case insert_mode::progressive: return base::insert_progressive_solid_intervals({ start }, { finish }, { true }, 1);
				case insert_mode::maximum: return base::insert_maximum_solid_intervals({ start }, { finish }, { true }, 1);
				case insert_mode::fluid: return base::insert_fluid_solid_intervals({ start }, { finish }, { true }, 1);
				default: return;
			};
		}

		void insert(std::vector<K> const& start, std::vector<K> const& finish) {
			auto n = start.size();
			if (n != finish.size())
				throw std::invalid_argument("interval set insert: size mismatch.");
			auto vals = std::vector<bool>(n, true);
			base::m_is_cache_valid = false;
			switch (base::m_mode)
			{
				case insert_mode::conservative: return base::insert_conservative_solid_intervals(start, finish, vals, n);
				case insert_mode::progressive: return base::insert_progressive_solid_intervals(start, finish, vals, n);
				case insert_mode::maximum:  return base::insert_maximum_solid_intervals(start, finish, vals, n);
				case insert_mode::fluid:  return base::insert_fluid_solid_intervals(start, finish, vals, n);
				default: return;
			};
		}
	};
}

#endif

