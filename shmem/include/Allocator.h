#ifndef HW_SHMEM_ALLOCATOR_H
#define HW_SHMEM_ALLOCATOR_H

#include <cstdint>
#include <memory>
#include <iostream>

namespace HW {

    namespace shmem {

        struct AllocState{
            uint8_t *m_freeMemBegin;
            uint8_t *m_freeMemEnd;

            AllocState()
            : m_freeMemBegin{nullptr}
            , m_freeMemEnd{nullptr} {}

            AllocState(uint8_t *shmem, std::size_t size)
            : m_freeMemBegin{shmem}
            , m_freeMemEnd{shmem + size} {}

            AllocState(const AllocState &rhs)
            : m_freeMemBegin{rhs.m_freeMemBegin}
            , m_freeMemEnd{rhs.m_freeMemEnd} {}

            AllocState &operator=(const AllocState &rhs) {
                m_freeMemBegin = rhs.m_freeMemBegin;
                m_freeMemEnd = rhs.m_freeMemEnd;
                return *this;
            }

            bool operator==(const AllocState &rhs) {
                return (m_freeMemBegin  == rhs.m_freeMemBegin && 
                        m_freeMemEnd    == rhs.m_freeMemEnd);
            }

            bool operator!=(const AllocState &rhs) {
                return !(operator==(rhs));
            }
        };

        template<class Type>
        struct allocator{
			using value_type = Type;
			using  pointer = Type*;
			using const_pointer = const Type*;
			using reference = Type& ;

            AllocState *m_state;

            template<class NewType>
            struct rebind {
                using other = allocator<NewType>;
            };

            allocator()
            : m_state{nullptr} {}

            allocator(AllocState *state)
            : m_state{state} {}

            [[nodiscard]] Type *allocate(std::size_t n) {
                if (!m_state) {
                    throw std::bad_alloc{};
                }
                if (m_state->m_freeMemBegin + n * sizeof(Type) > m_state->m_freeMemEnd) {
                    throw std::bad_alloc{};
                }
                uint8_t *begin = m_state->m_freeMemBegin;
                m_state->m_freeMemBegin += (sizeof(Type) * n);
			    return reinterpret_cast<Type*>(begin);
            }

            void deallocate(Type *p, std::size_t n) {
                if (!m_state) {
                    return;
                }
                if (reinterpret_cast<uint8_t*>(p) + sizeof(Type) * n == m_state->m_freeMemBegin) {
                    m_state->m_freeMemBegin = reinterpret_cast<uint8_t*>(p);
                }
            }

			template<class NewType>
			allocator(const allocator<NewType> &rhs)
            : m_state{rhs.m_state} {}

			template<class NewType>
			allocator &operator=(const allocator<NewType> &rhs) {
				m_state = rhs.m_state;
				return *this;
			}

            template<class OtherType>
            bool operator==(const allocator<OtherType> &rhs) {
                if (!m_state && !rhs.m_state) {
                    return true;
                }
                if (!m_state || !rhs.m_state) {
                    return false;
                }
                return (m_state == rhs.m_state);
            }

            template<class OtherType>
            bool operator!=(const allocator<OtherType> &rhs) {
                if (!m_state && !rhs.m_state) {
                    return false;
                }
                if (!m_state || !rhs.m_state) {
                    return true;
                }
                return (m_state != rhs.m_state);
            }

			~allocator() = default;

        };

    } // shmem

} // HW

#endif // HW_SHMEM_ALLOCATOR_H