#ifndef HW_LRUCACHE_H
#define HW_LRUCACHE_H

#include <unordered_map>
#include <list>

struct A {
    int x;
};

namespace HW {

    template<class Entry>
    class LRUCache {
    private:
        using Key = uint64_t;
        using CacheEntry = std::pair<Key, Entry*>;
        using pCache = typename std::list<CacheEntry>::iterator;

        std::list<CacheEntry> m_cache;
        std::unordered_map<Key, pCache> m_cacheMap;
        size_t m_cacheSize;

        void removeLRU() {
            CacheEntry last = m_cache.back();
            m_cache.pop_back();
            m_cacheMap.erase(last.first);
        }

        void insert(Key key, Entry *entry) {
            pCache newValue = m_cache.insert(m_cache.begin(), { key, entry });
            m_cacheMap[key] = newValue;
        }

    public:
        LRUCache(size_t cacheSize): m_cacheSize{cacheSize} {}

        Entry *get(Key key) {
            auto i = m_cacheMap.find(key);
            if (i == m_cacheMap.end()) {
                return nullptr;
            }
            Entry *res = i->second->second;
            remove(key);
            insert(key, res);
            return res;
        }

        void add(Key key, Entry *entry) {
            remove(key);
            if (m_cache.size() >= m_cacheSize) {
                removeLRU();
            }
            insert(key, entry);
        }

        void remove(Key key) {
            auto i = m_cacheMap.find(key);
            if (i != m_cacheMap.end()) {
                m_cache.erase(i->second);
                m_cacheMap.erase(key);
            }
        }
    };

} // HW

#endif // HW_LRUCACHE_H