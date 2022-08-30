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
      constexpr KVPair(const Key& k, const Value& v) : key{ k }, value{ v } {}

      /**
       * @brief Operator <=>
       * @param other KVPair to compare against.
       * @return true if the current key is less than the other's key.
       */
      [[nodiscard]] constexpr bool operator<=>(const KVPair<Key, Value>& other) const
      {
         return key <=> other.key;
      }

      Key   key{};   /**< Key. */
      Value value{}; /**< Associated value. */
   };
} // namespace rw::dat

#endif // KVPAIR_HPP