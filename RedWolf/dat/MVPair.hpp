#ifndef RW_DAT_MVPAIR_HPP
#define RW_DAT_MVPAIR_HPP

#include <mutex>

namespace rw::dat
{
   /**
    * @brief Mutex-value pair.
    * @tparam T Type of value to store.
    */
   template<typename T>
   struct MVPair
   {
      std::mutex mtx;
      T          value;
   };
} // namespace rw::dat

#endif