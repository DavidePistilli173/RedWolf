#ifndef RW_CONT_CONSTEXPRMAP_HPP
#define RW_CONT_CONSTEXPRMAP_HPP

#include "RedWolf/alg/sorting.hpp"
#include "RedWolf/dat/KVPair.hpp"

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace rw::cont
{
   /**
    * @brief Constexpr map datastructure.
    */
   template<typename Key, typename Value, size_t Capacity, typename Compare = std::less<Key>>
   class ConstexprMap
   {
   public:
      /**
       * @brief Custom iterator for the container.
       * @tparam ValueType Type of data on which to iterate.
       */
      template<typename ValueType>
      class Iterator
      {
      public:
         using iterator_category = std::random_access_iterator_tag;
         using difference_type = std::ptrdiff_t;
         using value_type = ValueType;
         using pointer = value_type*;
         using reference = value_type&;

         /**
          * @brief Construct an iterator starting from a specific pointer to the data.
          * @param ptr Pointer to start from.
          */
         explicit constexpr Iterator(pointer ptr) : ptr_{ ptr } {}

         /**
          * @brief Operator +=.
          * @param n Offset to add to the iterator.
          * @return Reference to the modified iterator.
          */
         constexpr Iterator& operator+=(difference_type n)
         {
            ptr_ += n;
            return *this;
         }

         /**
          * @brief Operator -=
          * @param n Offset to subtract from the iterator.
          * @return Reference to the modified iterator.
          */
         constexpr Iterator& operator-=(difference_type n)
         {
            ptr_ -= n;
            return *this;
         }

         /**
          * @brief Operator -
          * @param n Offset to subtract from the iterator.
          * @return New iterator with the modified value.
          */
         [[nodiscard]] constexpr Iterator operator-(difference_type n) const
         {
            return Iterator(ptr_ - n);
         }

         /**
          * @brief Operator -
          * @param it Iterator to subtract from this iterator.
          * @return Difference between the iterators.
          */
         [[nodiscard]] constexpr difference_type operator-(const Iterator& it) const
         {
            return ptr_ - it.ptr_;
         }

         /**
          * @brief Pre-decrement operator.
          * @return Iterator decremented by one.
          */
         constexpr Iterator& operator--()
         {
            --ptr_;
            return *this;
         }

         /**
          * @brief Post-decrement operator.
          * @return Iterator with the current value.
          */
         constexpr Iterator operator--(int)
         {
            Iterator result{ *this };

            operator--();

            return result;
         }

         /**
          * @brief Pre-increment operator.
          * @return Iterator incremented by one.
          */
         constexpr Iterator& operator++()
         {
            ++ptr_;
            return *this;
         }

         /**
          * @brief Post-increment operator.
          * @return Iterator with the current value.
          */
         constexpr Iterator operator++(int)
         {
            Iterator result{ *this };

            operator++();

            return result;
         }

         /**
          * @brief Operator []
          * @param n Offset from the current iterator.
          * @return Reference to the value pointed by the iterator + n.
          */
         [[nodiscard]] constexpr reference operator[](difference_type n) const
         {
            return *(ptr_ + n);
         }

         /**
          * @brief Operator <
          * @param it Iterator to compare agaist.
          * @return true or false.
          */
         [[nodiscard]] constexpr bool operator<(const Iterator& it) const
         {
            return ptr_ < it.ptr_;
         }

         /**
          * @brief Operator >
          * @param it Iterator to compare agaist.
          * @return true or false.
          */
         [[nodiscard]] constexpr bool operator>(const Iterator& it) const
         {
            return ptr_ > it.ptr_;
         }

         /**
          * @brief Operator <=
          * @param it Iterator to compare agaist.
          * @return true or false.
          */
         [[nodiscard]] constexpr bool operator<=(const Iterator& it) const
         {
            return ptr_ <= it.ptr_;
         }

         /**
          * @brief Operator >=
          * @param it  Iterator to compare agaist.
          * @return true or false.
          */
         [[nodiscard]] constexpr bool operator>=(const Iterator& it) const
         {
            return ptr_ >= it.ptr_;
         }

         /**
          * @brief Operator ==
          * @param it Iterator to compare agaist.
          * @return true or false.
          */
         [[nodiscard]] constexpr bool operator==(const Iterator& it) const
         {
            return ptr_ == it.ptr_;
         }

         /**
          * @brief Operator !=
          * @param it Iterator to compare against.
          * @return true or false.
          */
         [[nodiscard]] constexpr bool operator!=(const Iterator& it) const
         {
            return ptr_ != it.ptr_;
         }

         /**
          * @brief Operator *
          * @return Reference to the pointed element.
          */
         [[nodiscard]] constexpr reference operator*() const
         {
            return *ptr_;
         }

         /**
          * @brief Operator ->
          * @return Pointer to the current element.
          */
         [[nodiscard]] constexpr pointer operator->() const
         {
            return ptr_;
         }

         /**
          * @brief Operator +
          * @param it Iterator to add the offset to.
          * @param n Offset to add to the iterator.
          * @return New iterator with the modified value.
          */
         [[nodiscard]] constexpr friend Iterator operator+(const Iterator& it, difference_type n)
         {
            return (Iterator(it.ptr_ + n));
         }

         /**
          * @brief Operator +
          * @param n Offset to add to the iterator.
          * @param it Iterator to add the offset to.
          * @return New iterator with the modified value.
          */
         [[nodiscard]] constexpr friend Iterator operator+(difference_type n, const Iterator& it)
         {
            return (Iterator(it.ptr_ + n));
         }

      private:
         pointer ptr_; /**< Pointer to the current element. */
      };

      using key_type = Key;
      using mapped_type = Value;
      using value_type = rw::dat::KVPair<Key, Value>;
      using size_type = size_t;
      using difference_type = std::ptrdiff_t;
      using key_compare = Compare;
      using reference = value_type&;
      using const_reference = const value_type&;
      using pointer = value_type*;
      using const_pointer = const value_type*;
      using iterator = Iterator<value_type>;
      using const_iterator = Iterator<const value_type>;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;
      using value_compare = Compare;

      /**
       * @brief Default constructor. Constructs an empty map.
       */
      explicit constexpr ConstexprMap(Compare comp = Compare()) : comp_{ comp } {}

      /**
       * @brief Construct a map from a list of (key, value) pairs.
       * @param initData List of initial (key, value) pairs.
       */
      explicit constexpr ConstexprMap(std::initializer_list<value_type> initData, Compare comp = Compare()) :
         size_{ std::min(initData.size(), Capacity) }, comp_{ comp }
      {
         auto it = initData.begin();
         for (size_t i = 0; i < size_; ++i)
         {
            data_[i + 1] = *it++;
         }

         std::sort(begin(), end(), [this](const_reference val1, const_reference val2) { return comp_(val1.key, val2.key); });
      }

      /**
       * @brief Operator []
       * @param key Key to look for.
       * @return Reference to the value with the matching key.
       */
      [[nodiscard]] constexpr Value& operator[](const Key& key)
      {
         return internal_find_(key)->value;
      }

      /**
       * @brief Operator []
       * @param key Key to look for.
       * @return Read-only reference to the value pair with the matching key.
       */
      [[nodiscard]] constexpr const Value& operator[](const Key& key) const
      {
         return const_internal_find_(key)->value;
      }

      /**
       * @brief Returns a reference to the mapped value with a given key. Throws std::out_of_range if it does not exist.
       * @param key Key to look for.
       * @return Reference to the mapped value with the given key.
       */
      [[nodiscard]] constexpr Value& at(const Key& key)
      {
         auto element = internal_find_(key);
         if (element == end()) throw std::out_of_range("Key does not exist.");
         return element->value;
      }

      /**
       * @brief Returns a reference to the mapped value with a given key. Throws std::out_of_range if it does not exist.
       * @param key Key to look for.
       * @return Reference to the mapped value with the given key.
       */
      [[nodiscard]] constexpr const Value& at(const Key& key) const
      {
         auto element = const_internal_find_(key);
         if (element == cend()) throw std::out_of_range("Key does not exist.");
         return element->value;
      }

      /**
       * @brief Returns an iterator to the first element of the map.
       * @return Iterator to the first element of the map.
       */
      [[nodiscard]] constexpr iterator begin()
      {
         return iterator(&data_[1]);
      }

      /**
       * @brief Returns a read-only iterator to the first element of the map.
       * @return Read-only iterator to the first element of the map.
       */
      [[nodiscard]] constexpr const_iterator begin() const
      {
         return const_iterator(&data_[1]);
      }

      /**
       * @brief Returns a read-only iterator to the first element of the map.
       * @return Read-only iterator to the first element of the map.
       */
      [[nodiscard]] constexpr const_iterator cbegin() const
      {
         return const_iterator(&data_[1]);
      }

      /**
       * @brief Returns a read-only iterator to the element following the last element of the map.
       * @return Read-only iterator to the element following the last element of the map. Accessing it is undefined behaviour.
       */
      [[nodiscard]] constexpr const_iterator cend() const
      {
         return const_iterator(&data_[size_ + 1]);
      }

      /**
       * @brief Checks whether the map contains a given key or not.
       * @param key Key to look for.
       * @return true if the key is contained in the map, false otherwise.
       */
      [[nodiscard]] constexpr bool contains(const Key& key) const
      {
         return const_internal_find_(key) != cend();
      }

      /**
       * @brief Clear the contents of the map.
       */
      constexpr void clear()
      {
         for (size_t i = 1; i < size_; ++i)
         {
            data_[i] = rw::dat::KVPair(Key(), Value());
         }
         size_ = 0;
      }

      /**
       * @brief Returns a read-only reverse iterator to the last element of the map.
       * @return Read-only reverse iterator to the last element of the map.
       */
      [[nodiscard]] constexpr const_reverse_iterator crbegin() const
      {
         return const_reverse_iterator(const_iterator(&data_[size_ + 1]));
      }

      /**
       * @brief Returns a read-only reverse iterator to the element preceding the first element of the map.
       * @return Read-only reverse iterator to the element preceding the first element of the map. Accessing it is undefined behaviour.
       */
      [[nodiscard]] constexpr const_reverse_iterator crend() const
      {
         return const_reverse_iterator(const_iterator(&data_[1]));
      }

      /**
       * @brief Insert an element into the map, if no element with the same key exists, constructing it in place.
       * @param args Arguments to forward to the constructor of the element (Key, Value).
       * @return Iterator to the newly inserted element and true if the insertion took place, false otherwise.
       */
      template<typename... Args>
      constexpr std::pair<iterator, bool> emplace(Args&&... args)
      {
         if (size_ >= Capacity - 2) return std::make_pair(end(), false);

         auto value = value_type(std::forward<Args>(args)...);
         auto element = internal_find_(value.key);
         if (element != end()) return std::make_pair(element, false);

         data_[size_ + 1] = std::move(value);
         ++size_;
         return std::make_pair(
            rw::alg::sortLastElement(
               begin(), end(), [this](const_reference val1, const_reference val2) { return comp_(val1.key, val2.key); }),
            true);
      }

      /**
       * @brief Checks whether the map is empty or not.
       * @return true if the map is empty, false otherwise.
       */
      [[nodiscard]] constexpr bool empty() const
      {
         return size_ == 0;
      }

      /**
       * @brief Returns an iterator to the element following the last element of the map.
       * @return Iterator to the element following the last element of the map. Accessing it is undefined behaviour.
       */
      [[nodiscard]] constexpr iterator end()
      {
         return iterator(&data_[size_ + 1]);
      }

      /**
       * @brief Returns a read-only iterator to the element following the last element of the map.
       * @return Read-only iterator to the element following the last element of the map. Accessing it is undefined behaviour.
       */
      [[nodiscard]] constexpr const_iterator end() const
      {
         return const_iterator(&data_[size_ + 1]);
      }

      /**
       * @brief Remove an element from the map.
       * @param key Key to remove.
       * @return Value associated to the removed key.
       */
      constexpr Value erase(const Key& key)
      {
         auto element = internal_find_(key);
         if (element == end()) return Value();

         Value result = element->value;
         while (element != end())
         {
            *element = *(element + 1);
            ++element;
         }
         --size_;

         return result;
      }

      /**
       * @brief Find an element with a given key.
       * @param key Key to look for.
       * @return Iterator pointing to the searched key. Returns end if the key does not exist.
       */
      [[nodiscard]] constexpr iterator find(const Key& key)
      {
         return internal_find_(key);
      }

      /**
       * @brief Find an element with a given key.
       * @param key Key to look for.
       * @return Read-only iterator pointing to the searched key. Returns end if the key does not exist.
       */
      [[nodiscard]] constexpr const_iterator find(const Key& key) const
      {
         return const_internal_find_(key);
      }

      /**
       * @brief Insert an element into the map, if no element with the same key exists.
       * @param value Key, Value pair to insert.
       * @return Iterator to the newly inserted element and true if the insertion took place, false otherwise.
       */
      constexpr std::pair<iterator, bool> insert(const_reference value)
      {
         if (size_ >= Capacity - 2) return std::make_pair(end(), false);

         auto element = internal_find_(value.key);
         if (element != end()) return std::make_pair(element, false);

         data_[size_ + 1] = value;
         ++size_;
         return std::make_pair(
            rw::alg::sortLastElement(
               begin(), end(), [this](const_reference val1, const_reference val2) { return comp_(val1.key, val2.key); }),
            true);
      }

      /**
       * @brief Insert an element into the map or overwrite that with the same key.
       * @param value Key, Value pair to insert.
       * @return Iterator to the newly inserted element and true if the insertion took place, false otherwise.
       */
      constexpr std::pair<iterator, bool> insert_or_assign(const_reference value)
      {
         if (size_ >= Capacity - 2) return std::make_pair(end(), false);

         auto element = internal_find_(value.key);
         if (element != end())
         {
            element->value = value.value;
            return std::make_pair(element, true);
         }

         data_[size_ + 1] = value;
         ++size_;
         return std::make_pair(
            rw::alg::sortLastElement(
               begin(), end(), [this](const_reference val1, const_reference val2) { return comp_(val1.key, val2.key); }),
            true);
      }

      /**
       * @brief Returns an iterator pointing to the first element that is not less than (i.e. greater or equal to) key.
       * @param key Key to use as threshold.
       * @return Iterator pointing to the first element that is not less than (i.e. greater or equal to) key.
       */
      [[nodiscard]] constexpr iterator lower_bound(const Key& key)
      {
         return std::lower_bound(begin(), end(), key, [this](const value_type& val, const Key& key) { return comp_(val.key, key); });
      }

      /**
       * @brief Returns a read-only iterator pointing to the first element that is not less than (i.e. greater or equal to) key.
       * @param key Key to use as threshold.
       * @return Read-only iterator pointing to the first element that is not less than (i.e. greater or equal to) key.
       */
      [[nodiscard]] constexpr const_iterator lower_bound(const Key& key) const
      {
         return std::lower_bound(cbegin(), cend(), key, [this](const value_type& val, const Key& key) { return comp_(val.key, key); });
      }

      /**
       * @brief Get the maximum number of elements.
       * @return Maximum number of elements that the map can store.
       */
      [[nodiscard]] constexpr size_t max_size() const
      {
         return Capacity + 2;
      }

      /**
       * @brief Returns a reverse iterator to the last element of the map.
       * @return Reverse iterator to the last element of the map.
       */
      [[nodiscard]] constexpr reverse_iterator rbegin()
      {
         return reverse_iterator(iterator(&data_[size_ + 1]));
      }

      /**
       * @brief Returns a read-only reverse iterator to the last element of the map.
       * @return Read-only reverse iterator to the last element of the map.
       */
      [[nodiscard]] constexpr const_reverse_iterator rbegin() const
      {
         return const_reverse_iterator(iterator(&data_[size_ + 1]));
      }

      /**
       * @brief Returns a reverse iterator to the element preceding the first element of the map.
       * @return Reverse iterator to the element preceding the first element of the map. Accessing it is undefined behaviour.
       */
      [[nodiscard]] constexpr reverse_iterator rend()
      {
         return reverse_iterator(iterator(&data_[1]));
      }

      /**
       * @brief Returns a reverse iterator to the element preceding the first element of the map.
       * @return Reverse iterator to the element preceding the first element of the map. Accessing it is undefined behaviour.
       */
      [[nodiscard]] constexpr const_reverse_iterator rend() const
      {
         return const_reverse_iterator(iterator(&data_[1]));
      }

      /**
       * @brief Get the number of elements of the map.
       * @return Number of elements of the map.
       */
      [[nodiscard]] constexpr size_t size() const
      {
         return size_;
      }

      /**
       * @brief Returns an iterator pointing to the first element that is greater than key.
       * @param key Key to use as threshold.
       * @return Iterator pointing to the first element that is greater than key.
       */
      [[nodiscard]] constexpr iterator upper_bound(const Key& key)
      {
         return std::upper_bound(begin(), end(), key, [this](const Key& key, const value_type& val) { return comp_(key, val.key); });
      }

      /**
       * @brief Returns a read-only iterator pointing to the first element that is greater than key.
       * @param key Key to use as threshold.
       * @return Read-only iterator pointing to the first element that is greater than key.
       */
      [[nodiscard]] constexpr const_iterator upper_bound(const Key& key) const
      {
         return std::upper_bound(cbegin(), cend(), key, [this](const Key& key, const value_type& val) { return comp_(key, val.key); });
      }

   private:
      /**
       * @brief Internal utility function for finding an element with a given key.
       * @param key Key to look for.
       * @return Pointer to the given key, nullptr if not found.
       */
      [[nodiscard]] constexpr const_iterator const_internal_find_(const Key& key) const
      {
         auto element =
            std::lower_bound(cbegin(), cend(), key, [this](const value_type& val, const Key& key) { return comp_(val.key, key); });
         if (!(element == cend()) && !(comp_(key, element->key))) return element;
         return cend();
      }

      /**
       * @brief Internal utility function for finding an element with a given key.
       * @param key Key to look for.
       * @return Pointer to the given key, nullptr if not found.
       */
      [[nodiscard]] constexpr iterator internal_find_(const Key& key)
      {
         auto element =
            std::lower_bound(begin(), end(), key, [this](const value_type& val, const Key& key) { return comp_(val.key, key); });
         if (!(element == end()) && !(comp_(key, element->key))) return element;
         return end();
      }

      /**
       * @brief Comparator for value_types and keys.
       * @param val Left value.
       * @param key Right value.
       * @return true if left < right.
       */
      [[nodiscard]] constexpr bool valKeyComp_(const value_type& val, const Key& key) const
      {
         return comp_(val.key, key);
      }

      std::array<rw::dat::KVPair<Key, Value>, Capacity + 2> data_;              /**< Map data. Actual elements go from 1 to size_ - 1. */
      size_t                                                size_{ 0U };        /**< Number of elements of the map. */
      Compare                                               comp_{ Compare() }; /**< Comparator for keys. */
   };
} // namespace rw::cont

#endif // MAP_HPP
