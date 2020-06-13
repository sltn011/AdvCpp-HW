#ifndef HW_FILE_READER_H
#define HW_FILE_READER_H

#include "bigFile/Mmap.hpp"
#include "data/LRUCache.hpp"

namespace HW::File{

    template<class Entry>
    class Reader {
    public:
        using Key = uint64_t;
        
    protected:
        Mmap m_mmap;

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
        Reader(std::string &path, bool isFileShared)
        : m_mmap{path, isFileShared} {}

        virtual Entry* getEntry(const Key key) {
            return binSearchEntry((Entry*)m_mmap.getData(), (Entry*)m_mmap.end(), key);
        }
    };

    template<class Entry>
    class LRUCacheReader : public Reader<Entry> {
    public:
        using Key = typename Reader<Entry>::Key;
       
    private:
        LRUCache<Entry> m_cache;

    public:
        LRUCacheReader(std::string &path, bool isFileShared, size_t cacheSize)
        : Reader<Entry>{path, isFileShared}
        , m_cache{cacheSize} {}

        Entry* getEntry(const Key key) override {
            Entry *cached = m_cache.get(key);
            if (cached) {
                return cached;
            }
            Entry *fromFile = this->binSearchEntry((Entry*)this->m_mmap.getData(), (Entry*)this->m_mmap.end(), key);
            if (fromFile) {
                m_cache.add(key, fromFile);
            }
            return fromFile;
        }
    };

} // HW::File

#endif // HW_FILE_READER_H 