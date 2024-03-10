#pragma once

#ifndef INCLUDE_UTILS_INTERVALMAP_H_
#define INCLUDE_UTILS_INTERVALMAP_H_

#include "../Tech.h"
#include "Utils.h"

template <typename K, typename V>
class interval_map {
	friend static class IntervalMapTest;

private:
	V m_valBegin;
	std::map<K, V> m_map;

public:

	interval_map(V const& val) : m_valBegin(val) {}

	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
		if (keyEnd <= keyBegin)
			return;

		auto startIt = m_map.lower_bound(keyBegin);					// O(log(n))
		auto finishIt = m_map.lower_bound(keyEnd);					// O(log(n))
		auto isFinishIn = m_map.find(keyEnd) == m_map.end();

		auto preBeginVal = startIt == m_map.begin() ? m_valBegin : std::prev(startIt)->second;

		if (startIt == m_map.end()) {
			if (preBeginVal != val) {
				m_map.insert(std::make_pair(keyBegin, val));		// O(log(n))
				m_map.insert(std::make_pair(keyEnd, preBeginVal));	// O(log(n))
			}
		}
		else {
			V preEndVal = preBeginVal;
			auto it = startIt;
			while (it != finishIt) {								// O(distance(start, finish))
				auto nextIt = std::next(it);
				if (nextIt == finishIt) {
					if (!isFinishIn)
						preEndVal = nextIt->second;
					else
						preEndVal = it->second;						// construction & assignments
				}
				it = m_map.erase(it);								// amortized O(1)
			}
			auto isBeginOk = preBeginVal != val;
			auto isFinishOk = (isBeginOk && std::prev(finishIt)->second != preEndVal)
				|| (!isBeginOk && preBeginVal != preEndVal);

			if (isBeginOk)
				m_map.insert(std::make_pair(keyBegin, val));		// O(log(n))
			if (isFinishOk)
				m_map.insert(std::make_pair(keyEnd, preEndVal));	// O(log(n))
		}
	}

	V const& operator[](K const& key) const {
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin()) {
			return m_valBegin;
		}
		else {
			return (--it)->second;
		}
	}
};

#endif
