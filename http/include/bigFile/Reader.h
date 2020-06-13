#ifndef HW_FILE_READER_H
#define HW_FILE_READER_H

#include "bigFile/Mmap.h"
#include "data/LRUCache.h"
#include <unordered_map>
#include <cstring>

namespace HW::File{

    template<class Entry>
    class Reader {
    public:
        using Key = uint64_t;
        using EntryPos = typename std::map<Key, Entry*>;
        
    private:
        Mmap m_mmap;
        EntryPos m_entryPos;
        uint64_t m_blockSize;
        LRUCache<Entry> m_cache;

        Entry *binSearchEntry(Entry* l, Entry* r, Key key) {
            while (l <= r) {
                Entry *m = l + (r-l)/2;
                Key mKey = *(reinterpret_cast<Key*>(m));
                if (mKey == key) {
                    return m;
                }
                else if (mKey < key) {
                    l = m + 1;
                }
                else {
                    r = m - 1;
                }
            }
            return nullptr;
        }

    public:
        Reader(std::string &path, uint64_t blockSize, size_t cacheSize, bool isFileShared)
        : m_mmap{path, isFileShared}
        , m_cache{cacheSize} {
            m_blockSize = (m_mmap.getSize()/sizeof(Entry) > blockSize) ? blockSize : m_mmap.getSize()/sizeof(Entry);
            uint64_t numEntries = static_cast<uint64_t>(m_mmap.getSize())/sizeof(Entry);
            Entry *data = static_cast<Entry*>(m_mmap.getData());
            for(uint64_t i = 0; i < numEntries; i += m_blockSize) {
                Key entryKey = *((Key*)(data + i));
                m_entryPos.emplace_hint(m_entryPos.end(), entryKey, data + i);
            }
        }

        Entry* getEntry(const Key key) {
            Entry *cached = m_cache.get(key);
            if (cached) {
                std::cout << "Got " << key << " from cache" << std::endl;
                return cached;
            }
            for (auto i = m_entryPos.rbegin(); i != m_entryPos.rend(); ++i) {
                if (i->first <= key) {
                    size_t elementsInBlock;
                    if (*i != *m_entryPos.rbegin()) {
                        elementsInBlock = m_blockSize;
                    }
                    else {
                        elementsInBlock = static_cast<Entry*>(m_mmap.end()) - i->second;
                        std::cout << elementsInBlock << std::endl;
                    }
                    Entry *fromFile = binSearchEntry(i->second, i->second + elementsInBlock, key);
                    if (fromFile) {
                        std::cout << "Adding " << key << " to cache" << std::endl;
                        m_cache.add(key, fromFile);
                    }
                    return fromFile;
                }
            }
            return nullptr;
        }
    };

} // HW::File

#endif // HW_FILE_READER_H 