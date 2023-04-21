#ifndef RW_DAT_KVPAIR_HPP
#define RW_DAT_KVPAIR_HPP

namespace rw::dat
{
   /**
    * @brief Constexpr (key, value) pair.
    */
   template<typename Key, typename Value>
   struct KVPair
   {
      /**
       * @brief Default constructor.
       */
      constexpr KVPair() = default;

      /**
       * @brief Constructor.
       * @param k Initial key.
       * @param v Initial value.
       */
      constexpr KVPair(const Key& k, const Value& v);

      /**
       * @brief Operator ==
       * @param other KVPair to compare against.
       * @return true if the current key is equal to the other key.
       */
      [[nodiscard]] constexpr bool operator==(const KVPair<Key, Value>& other) const;

      /**
       * @brief Operator <=>
       * @param other KVPair to compare against.
       * @return true if the current key is less than the other's key.
       */
      [[nodiscard]] constexpr auto operator<=>(const KVPair<Key, Value>& other) const;

      Key   key{};   /**< Key. */
      Value value{}; /**< Associated value. */
   };
} // namespace rw::dat

// IMPLEMENTATION
template<typename Key, typename Value>
constexpr rw::dat::KVPair<Key, Value>::KVPair(const Key& k, const Value& v) : key{ k }, value{ v }
{
}

template<typename Key, typename Value>
constexpr bool rw::dat::KVPair<Key, Value>::operator==(const KVPair<Key, Value>& other) const
{
   return key == other.key;
}

template<typename Key, typename Value>
constexpr auto rw::dat::KVPair<Key, Value>::operator<=>(const KVPair<Key, Value>& other) const
{
   return key <=> other.key;
}

#endif // KVPAIR_HPP
