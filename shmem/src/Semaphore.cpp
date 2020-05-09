#include "Semaphore.h"

namespace HW {

    namespace shmem {

        Semaphore::Semaphore() : m_sem{nullptr} {}

        Semaphore::Semaphore(sem_t *sem, bool shared, int init_val) : m_sem{nullptr} {
            create(sem, shared, init_val);
        }

        Semaphore::~Semaphore() noexcept {
            try {
                destroy();
            }
            catch (IPCError &e) {}
        }

        void Semaphore::create(sem_t *sem, bool shared, int init_val) {
            if (!m_sem) {
                if (sem_init(sem, static_cast<int>(shared), init_val) < 0) {
                    throw IPCError("Error initializing semaphore!");
                }
                m_sem = sem;
            }
        }

        void Semaphore::destroy() {
            if (!m_sem) {
                return;
            }
            if (sem_destroy(m_sem) < 0) {
                throw IPCError("Error destroying semaphor!");
            }
            m_sem = nullptr;
        }

        void Semaphore::wait() {
            if (!m_sem) {
                return;
            }
            if (sem_wait(m_sem) < 0) {
                throw IPCError("Error in sem_wait!");
            }
        }

        void Semaphore::post() {
            if (!m_sem) {
                return;
            }
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