#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>
#include <armadillo>
#include <omp.h>
#include <openblas/cblas.h>
#include "include/utils.h"

template <typename K, typename V>
class interval_map {
  friend void IntervalMapTest();

public:
  V m_valBegin;
  std::map<K,V> m_map;

  interval_map(V const& val) : m_valBegin(val) {}

  void assign(K const& keyBegin, K const& keyEnd, V const& val) {
    if (keyEnd <= keyBegin)
	return;

    auto startIt = m_map.lower_bound(keyBegin);		// O(log(n))
    auto finishIt = m_map.lower_bound(keyEnd);		// O(log(n))
    auto isFinishIn = m_map.find(keyEnd) == m_map.end();

    auto preBeginVal = startIt == m_map.begin()? m_valBegin : std::prev(startIt)->second;

    if (startIt == m_map.end()) {
	if (preBeginVal != val) {
	  m_map[keyBegin] = val;			// O(log(n))
	  m_map[keyEnd] = preBeginVal;			// O(log(n))
	}
    }
    else {
	V preEndVal = preBeginVal;
	auto it = startIt;
	while(it != finishIt) {				// O(distance(start, finish))
	    auto nextIt = std::next(it);
	    if (nextIt == finishIt) {
		if (!isFinishIn)
		  preEndVal = nextIt->second;
		else
		  preEndVal = it->second;               // construction & assignments
	    }
	    it = m_map.erase(it);			// amortized O(1)
	}
	auto isBeginOk = preBeginVal != val;
	auto isFinishOk = (isBeginOk && std::prev(finishIt)->second != preEndVal)
			|| (!isBeginOk && preBeginVal != preEndVal);

	if (isBeginOk)
	  m_map[keyBegin] = val;			// O(log(n))
	if (isFinishOk)
	  m_map[keyEnd] = preEndVal;			// O(log(n))
    }
  }

  V const& operator[](K const& key) const {
    auto it = m_map.upper_bound(key);
    if(it == m_map.begin()) {
	return m_valBegin;
    } else {
	return (--it)->second;
    }
  }
};

int main(int argc, char **argv) {
  interval_map<int, char> mymap ('A');

  mymap.assign(1, 3, 'B');
  mymap.assign(5, 7, 'C');
  mymap.assign(9, 11, 'D');
  mymap.assign(13, 17, 'E');
  mymap.assign(19, 21, 'F');

  mymap.assign(6, 20, 'Z');

  std::cout << std::endl;
  for(int i = -3; i < 30; i++) {
    std::cout << std::setw(5) << "  " + std::to_string(i) << "  "
	<< (mymap[i] == 'A'? "" : (mymap[i] == 'Z'? "  " : " "))
	<< mymap[i] << std::endl;
  }

  std::cout << std::endl;
  for(auto it : mymap.m_map) {
      std::cout << " " << it.first << " " << it.second << "  ";
  }

  std::cout << std::endl << std::endl;
  return 0;
}


