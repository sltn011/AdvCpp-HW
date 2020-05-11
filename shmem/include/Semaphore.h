#ifndef HW_SHMEM_SEM_H
#define HW_SHMEM_SEM_H

#include <semaphore.h>
#include "Exceptions.h"

namespace HW {

    namespace shmem {

        class Semaphore {
        private:
            sem_t *m_sem;

        public:
            Semaphore(sem_t *sem, bool shared, int init_val);
            ~Semaphore() noexcept;

            void wait();
            void post();
        };

        class SemaphoreLock {
        private:
            Semaphore &m_sem;

        public:
            SemaphoreLock(Semaphore &sem);
            ~SemaphoreLock();
        };

    } // shmem

} // HW


#endif // HW_SHMEM_MEM_H