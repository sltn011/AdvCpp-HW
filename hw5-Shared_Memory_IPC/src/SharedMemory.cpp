#include "SharedMemory.hpp"

namespace HW {

    namespace shmem {

        SharedMemory::SharedMemory(std::size_t size)
        : m_memBegin{nullptr}
        , m_memEnd{nullptr} {
            size += (sizeof(Semaphore) +sizeof(sem_t));
            void *pShMem = (uint8_t*)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            if (pShMem == MAP_FAILED) {
                throw IPCError("Error creating shared memory!");
            }
            m_memBegin = (uint8_t*)pShMem;
            m_memEnd = m_memBegin + size;
            try {
                m_sem = new(m_memBegin) Semaphore(reinterpret_cast<sem_t*>(m_memBegin + sizeof(Semaphore)), true, 1);
                m_memBegin += (sizeof(Semaphore) +sizeof(sem_t));
            }
            catch (IPCError &e) {
                munmap(m_memBegin, m_memEnd-m_memBegin);
                throw;
            }
            m_creatorPID = getpid();
        }

        SharedMemory::~SharedMemory() noexcept {
            if (m_creatorPID != getpid()) {
                return;
            }
            m_sem->~Semaphore();
            m_memBegin -= (sizeof(Semaphore) +sizeof(sem_t));
            munmap(m_memBegin, m_memEnd-m_memBegin);
        }

    } // shmem

} // HW