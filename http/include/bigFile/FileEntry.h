#ifndef HW_FILE_ENTRY_H
#define HW_FILE_ENTRY_H

#include <cstdint>

namespace HW::File{

    using Key = uint64_t;
    using Data = char[20];

    struct Entry {
        Key m_key;
        Data m_data;
    };

} // HW::File

#endif // HW_FILE_ENTRY_H 