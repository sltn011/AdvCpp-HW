#include <fstream>
#include <inttypes.h>
#include <string_view>
#include <cstring>

    using Key = uint64_t;
    using Data = char[20];

    struct Entry {
        Key m_key;
        Data m_data;
    };

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Use ./fileGenerator NUM_ENTRIES\n");
    }
    uint64_t numEntries;
    constexpr Data msg{'A', 'B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T'};
    sscanf(argv[1], "%" SCNu64, &numEntries);
    std::ofstream f("data.bin", std::ios::binary);
    Entry entry;
    memcpy(entry.m_data, &msg[0], sizeof(msg));
    for (uint64_t i = 0; i < numEntries; ++i) {
        entry.m_key = i;
        f.write(reinterpret_cast<char*>(&entry), sizeof(entry));
    }
    f.close();
}