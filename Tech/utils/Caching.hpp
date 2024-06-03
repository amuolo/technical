#pragma once

#ifndef INCLUDE_UTILS_CACHING_LRU_H_
#define INCLUDE_UTILS_CACHING_LRU_H_

#include "StandardLibs.hpp"

namespace tech
{
	/**
	 * A cache is a component that stores data so future requests for that data can be served faster. 
	 * The data stored in a cache might be the results of an earlier computation, or the duplicates of 
	 * data stored elsewhere. A cache hit occurs when the requested data can be found in a cache, 
	 * while a cache miss occurs when it cannot. Cache hits are served by reading data from the cache 
	 * which is faster than recomputing a result or reading from a slower data store. 
	 * Thus, the more requests that can be served from the cache, the faster the system performs.
	 * One of the popular cache replacement policies is: "least recently used" (LRU). 
	 * It discards the least recently used items first.
	 */

	template <typename K, typename V>
	struct Node {
		Node* next;
		Node* prev;
		V value;
		K key;
		Node(Node* p, Node* n, int k, int val) : prev(p), next(n), key(k), value(val) {};
		Node(K k, V val) :prev(nullptr), next(nullptr), key(k), value(val) {};
	};

	template <typename K, typename V>
	class Cache {
	protected:
		std::unordered_map<K, Node<K, V>*> m_map;
		int m_capacity;
		Node<K, V>* tail; 
		Node<K, V>* head; 
		virtual void set(K, V) = 0; 
		virtual std::pair<bool, V const* const> get(K) = 0;
	};

	template <typename K, typename V>
	class cache_lru : public Cache<K, V> {

        using base = Cache<K, V>;

    public:
        cache_lru(size_t capacity) {
            base::m_capacity = capacity;
        };

        void set(K key, V value) {
            const auto& it = base::m_map.find(key);
            if (it != base::m_map.end()) {
                it->second->value = value;
                return;
            }

            auto node = new Node<K, V>(nullptr, nullptr, key, value);
            if (base::m_map.size() == 0) {
                base::m_map.insert(std::make_pair(key, node));
                base::tail = node;
                base::head = node;
            }
            else {
                base::head->prev = node;
                node->next = base::head;
                base::m_map.insert(std::make_pair(key, node));
                base::head = node;

                if (base::m_map.size() > base::m_capacity) {
                    auto& newTail = base::tail->prev;
                    newTail->next = nullptr;
                    base::m_map.erase(base::tail->key);
                    delete base::tail;
                    base::tail = newTail;
                }
            }
        }

        std::pair<bool, V const * const> get(K key) {
            const auto& it = base::m_map.find(key);
            if (it != base::m_map.end())
                return std::make_pair(true, &(it->second->value));
            else
                return std::make_pair(false, nullptr);
        }
    };
}

#endif


