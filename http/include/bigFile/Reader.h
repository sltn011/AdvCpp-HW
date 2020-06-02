#ifndef HW_FILE_READER_H
#define HW_FILE_READER_H

#include "bigFile/Mmap.h"
#include <unordered_map>
#include <cstring>

namespace HW::File{

    template<class Entry>
    class Reader {
    public:
        using Key = uint64_t;
        using EntryPos = typename std::unordered_map<Key, Entry*>;
        
    private:
        Mmap m_mmap;
        EntryPos m_entryPos;

    public:
        Reader(std::string &path, bool isFileShared): m_mmap{path, isFileShared} {
            uint64_t numEntries = static_cast<uint64_t>(m_mmap.getSize())/sizeof(Entry);
            Entry *data = (Entry*)m_mmap.getData();
            for(uint64_t i = 0; i < numEntries; ++i) {
                Key entryKey = *((Key*)(data + i));
                m_entryPos[entryKey] = data + i;
            }
        }

        Entry* getEntry(const Key key) {
            typename EntryPos::const_iterator p = m_entryPos.find(key);
            if (p == m_entryPos.end()) {
                return nullptr;
            }
            return p->second;
        }
    };

} // HW::File

#endif // HW_FILE_READER_H 