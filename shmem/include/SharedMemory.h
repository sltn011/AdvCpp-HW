#ifndef HW_SHMEM_MEM_H
#define HW_SHMEM_MEM_H

#include <sys/mman.h>
#include <cstdint>
#include <unistd.h>
#include "Semaphore.h"

namespace HW {

    namespace shmem {
        
        struct SharedMemory {
            uint8_t     *m_memBegin;
            uint8_t     *m_memEnd;
            pid_t        m_creatorPID;
            Semaphore   *m_sem;

            SharedMemory(std::size_t size);
            ~SharedMemory() noexcept;
        };

    } // shmem

} // HW


#endif // HW_SHMEM_MEM_H