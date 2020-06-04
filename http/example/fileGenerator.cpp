#include <fstream>
#include <inttypes.h>
#include <string_view>
#include <cstring>
#include "bigFile/FileEntry.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Use ./fileGenerator NUM_ENTRIES\n");
    }
    uint64_t numEntries;
    constexpr HW::File::Data msg{'A', 'B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T'};
    sscanf(argv[1], "%" SCNu64, &numEntries);
    std::ofstream f("data.bin", std::ios::binary);
    HW::File::Entry entry;
    memcpy(entry.m_data, msg, sizeof(entry.m_data));
    for (HW::File::Key i = 0; i < numEntries; ++i) {
        entry.m_key = i;
        f.write(reinterpret_cast<char*>(&entry), sizeof(entry));
    }
    f.close();
}