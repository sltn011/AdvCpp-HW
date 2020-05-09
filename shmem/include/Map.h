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
            using iterator                  = typename Map_t::iterator;
            using const_iterator            = typename Map_t::const_iterator;
            using reverse_iterator          = typename Map_t::reverse_iterator;
            using const_reverse_iterator    = typename Map_t::const_reverse_iterator;
            using size_type                 = typename Map_t::size_type;
            using pSharedMemory             = std::unique_ptr<SharedMemory>;
            using pMap                      = Map_t*;

            pSharedMemory   m_mem;
            pMap            m_map;
            AllocState      m_alloc_state;
            alloc_t         m_alloc;
            
        public:
            Map(std::size_t size) : m_map{nullptr} {
                m_mem = std::make_unique<SharedMemory>(size + sizeof(Map_t));
                m_alloc_state = {m_mem->m_memBegin + sizeof(Map_t), size};
                m_alloc = alloc_t{&m_alloc_state};
                m_map = new(m_mem->m_memBegin) Map_t(m_alloc);
            }

            T operator[](const Key &key) {
                SemaphoreLock lock(m_mem->m_sem);
                if constexpr(std::is_pod<Key>::value) {
                    return T{m_map->operator[](key)};
                }
                else return T{m_map->operator[](std::move(GoodKey{key, m_alloc}))};
            }

            T operator[](Key &&key) {
                SemaphoreLock lock(m_mem->m_sem);
                if constexpr(std::is_pod<Key>::value) {
                    return T{m_map->operator[](key)};
                }
                else return T{m_map->operator[](std::move(GoodKey{key, m_alloc}))};
            }

            const_iterator cbegin() const {
                SemaphoreLock lock(m_mem->m_sem);
                return m_map->cbegin();
            }

            const_iterator cend() const {
                SemaphoreLock lock(m_mem->m_sem);
                return m_map->cend();
            }

            const_reverse_iterator crbegin() const {
                SemaphoreLock lock(m_mem->m_sem);
                return m_map->crbegin();
            }

            const_reverse_iterator crend() const {
                SemaphoreLock lock(m_mem->m_sem);
                return m_map->crend();
            }

            bool empty() const {
                SemaphoreLock lock(m_mem->m_sem);
                return m_map->empty();
            }

            size_type size() const {
                SemaphoreLock lock(m_mem->m_sem);
                return m_map->size();
            }

            void clear() {
                SemaphoreLock lock(m_mem->m_sem);
                m_map->clear();
            }

            void insert(const value_t &value) {
                SemaphoreLock(m_mem->m_sem);
                map_insert(std::forward<value_t>(value));
            }

            void insert(value_t &&value) {
                SemaphoreLock(m_mem->m_sem);
                map_insert(std::forward<value_t>(value));
            }

            template<class P>
            void insert(P &&value) {
                SemaphoreLock(m_mem->m_sem);
                map_insert(std::forward<value_t>(value));
            }

            const_iterator erase(const_iterator position) {
                SemaphoreLock(m_mem->m_sem);
                return m_map->erase(position);
            }

            const_iterator erase(const_iterator first, const_iterator last) {
                SemaphoreLock(m_mem->m_sem);
                return m_map->erase(first, last);
            }

            size_type erase(const Key &key) {
                SemaphoreLock(m_mem->m_sem);
                if constexpr(std::is_pod<Key>::value) {
                    return m_map->erase(key);
                }
                else return m_map->erase(std::move(GoodKey{key, m_alloc}));
            }

            size_type count(const Key &key) const {
                SemaphoreLock(m_mem->m_sem);
                if constexpr(std::is_pod<Key>::value) {
                    return m_map->count(key);
                }
                else return m_map->count(std::move(GoodKey{key, m_alloc}));
            }

            const_iterator find(const Key &key) {
                SemaphoreLock(m_mem->m_sem);
                if constexpr(std::is_pod<Key>::value) {
                    return m_map->find(key);
                }
                else return m_map->find(std::move(GoodKey{key, m_alloc}));
            }

            auto set_value(const value_t &val) {
                erase(val.first);
                return insert(value_t{val});
            }

        private:
            void map_insert(const value_t &val) {
                if constexpr (std::is_pod<Key>::value && std::is_pod<T>::value) {
                    m_map->insert(GoodValue_t{val});
                }
                else if constexpr (std::is_same<Key, std::string>::value && std::is_pod<T>::value) {
                    m_map->insert(GoodValue_t{GoodKey{val.first, m_alloc}, GoodT{val.second}});
                }
                else if constexpr (std::is_pod<Key>::value && std::is_same<T, std::string>::value) {
                    m_map->insert(GoodValue_t{GoodKey{val.first}, GoodT{val.second, m_alloc}});
                }
                else if constexpr (std::is_same<Key, std::string>::value && std::is_same<T, std::string>::value) {
                    m_map->insert(GoodValue_t{GoodKey{val.first, m_alloc}, GoodT{val.second, m_alloc}});
                }
                else throw HW::IOError("Error inserting value to map! Use POD-types or std::string");
            }

        };

        
        
    } // shmem

} // HW

#endif // HW_SHMEM_MAP_H