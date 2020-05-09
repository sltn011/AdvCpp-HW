#include "SharedMemory.h"

namespace HW {

    namespace shmem {

        SharedMemory::SharedMemory(std::size_t size)
        : m_memBegin{nullptr}, m_memEnd{nullptr} {
            create(size);
        }

        SharedMemory::~SharedMemory() noexcept {
            try {
                destroy();
            }
            catch (IPCError &e) {}
        }

        void SharedMemory::create(std::size_t size) {
            if (m_memBegin && m_memEnd) {
                return;
            }
            size += sizeof(sem_t);
            m_memBegin = (uint8_t*)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            if (m_memBegin == MAP_FAILED) {
                std::cout << errno << std::endl;
                m_memBegin = nullptr;
                throw IPCError("Error creating shared memory!");
            }
            m_memEnd = m_memBegin + size;
            try {
                m_sem.create(reinterpret_cast<sem_t*>(m_memBegin), true, 1);
                m_memBegin += sizeof(sem_t);
            }
            catch (IPCError &e) {
                munmap(m_memBegin, m_memEnd-m_memBegin);
                throw;
            }
            m_creatorPID = getpid();
        }

        void SharedMemory::destroy() {
            if (m_creatorPID != getpid()) {
                return;
            }
            if (!m_memBegin || !m_memEnd) {
                return;
            }
            m_sem.destroy();
            m_memBegin -= sizeof(sem_t);
            if (munmap(m_memBegin, m_memEnd-m_memBegin) < 0) {
                throw IPCError("Error destroying shared memory!");
            }
            m_memBegin = nullptr;
            m_memEnd = nullptr;
            m_creatorPID = -1;
        }

    } // shmem

} // HW