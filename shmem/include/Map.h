#ifndef HW_SHMEM_MAP_H
#define HW_SHMEM_MAP_H

#include <map>
#include "Allocator.h"
#include "SharedMemory.h"
#include <memory>

namespace HW {

    namespace shmem {

        using string_allocator = allocator<char>;
        using string = std::basic_string<char, std::char_traits<char>, string_allocator>;

        template<class T>
        struct check_dynamic {
            using type = typename std::conditional<std::is_pod<T>::value, T, string>::type;
        };

        template<
        class Key, 
        class T,
        class Compare = std::less<typename check_dynamic<Key>::type>
        >class Map {
        private:
            using value_t                   = std::pair<Key, T>;
            using GoodKey                   = typename check_dynamic<Key>::type;
            using GoodT                     = typename check_dynamic<T>::type;
            using GoodValue_t               = std::pair<GoodKey, GoodT>;
            using alloc_t                   = allocator<GoodValue_t>;
            using Map_t                     = std::map<GoodKey, GoodT, Compare, alloc_t>;
            using size_type                 = typename Map_t::size_type;
            using pSharedMemory             = std::unique_ptr<SharedMemory>;
            using pMap                      = Map_t*;
            using pAlloc                    = alloc_t*;

            pSharedMemory   m_mem;
            pMap            m_map;
            pAlloc          m_alloc;
            
        public:
            Map(std::size_t size) {
                m_mem = std::make_unique<SharedMemory>(size + sizeof(AllocState) + sizeof(alloc_t) + sizeof(Map_t));
                AllocState *state = new(m_mem->m_memBegin)
                    AllocState(m_mem->m_memBegin + sizeof(AllocState) + sizeof(alloc_t) + sizeof(Map_t), size);
                m_alloc = new(m_mem->m_memBegin + sizeof(AllocState))
                    alloc_t(*state);
                m_map = new(m_mem->m_memBegin + sizeof(AllocState) + sizeof(alloc_t)) Map_t(*m_alloc);
            }

            T operator[](const Key &key) {
                SemaphoreLock lock(*(m_mem->m_sem));
                return T{m_map->operator[](convertKey(key))};
            }

            void clear() {
                SemaphoreLock lock(*(m_mem->m_sem));
                m_map->clear();
            }

            void insert(const value_t &value) {
                SemaphoreLock lock(*(m_mem->m_sem));
                m_map->insert({convertKey(value.first), convertT(value.second)});
            }

            template<class P>
            void insert(P &&value) {
                SemaphoreLock lock(*(m_mem->m_sem));
                m_map->insert({convertKey(value.first), convertT(value.second)});
            }

            size_type erase(const Key &key) {
                SemaphoreLock lock(*(m_mem->m_sem));
                return m_map->erase(convertKey(key));
            }

            auto set_value(const value_t &val) {
                return m_map->operator[](convertKey(val.first)) = convertT(val.second);
            }

        private:
            GoodKey convertKey(const Key &key) {
                if constexpr (std::is_pod<Key>::value) {
                    return GoodKey{key};
                }
                else return GoodKey{key, *m_alloc};
            }

            GoodT convertT(const T &t) {
                if constexpr (std::is_pod<T>::value) {
                    return GoodT{t};
                }
                else return GoodT{t, *m_alloc};
            }

        };

        
        
    } // shmem

} // HW

#endif // HW_SHMEM_MAP_H