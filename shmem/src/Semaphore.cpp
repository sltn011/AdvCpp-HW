#include "Semaphore.h"

namespace HW {

    namespace shmem {

        Semaphore::Semaphore(sem_t *sem, bool shared, int init_val) : m_sem{nullptr} {
            if (sem_init(sem, static_cast<int>(shared), init_val) < 0) {
                throw IPCError("Error initializing semaphore!");
            }
            m_sem = sem;
        }

        Semaphore::~Semaphore() noexcept {
            sem_destroy(m_sem);
        }

        void Semaphore::wait() {
            if (sem_wait(m_sem) < 0) {
                throw IPCError("Error in sem_wait!");
            }
        }

        void Semaphore::post() {
            if (sem_post(m_sem) < 0) {
                throw IPCError("Error in sem_post!");
            }
        }

        SemaphoreLock::SemaphoreLock(Semaphore &sem) : m_sem{sem} {
            m_sem.wait();
        }

        SemaphoreLock::~SemaphoreLock() {
            m_sem.post();
        }

    } // shmem

} // HW