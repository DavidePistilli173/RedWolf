#ifndef RW_THREAD_MUTEX_HPP
#define RW_THREAD_MUTEX_HPP

#include "RedWolf/common.hpp"

#include <mutex>

namespace rw::thread
{
   /**
    * @brief Mutex wrapper that holds the value it protects.
    * @tparam T Type of object protected by the mutex.
    */
   template<typename T>
   class RW_API Mutex
   {
   public:
      /**
       * @brief Lock guard for accessing the protected object.
       */
      struct Guard
      {
         std::scoped_lock<std::mutex> lck; /**< Mutex lock. */
         T&                           val; /**< Reference to the protected value. */
      };

      /**
       * @brief Default constructor.
       */
      Mutex() = default;

      /**
       * @brief Construct an object and its protecting mutex.
       * @tparam ...Args Types of arguments to pass to the object's constructor.
       * @param ...args Arguments to pass to the object's constructor.
       */
      template<typename... Args>
      Mutex(Args&&... args);

      /**
       * @brief Lock the mutex and get the protected value.
       * @return Guard to the protected value.
       */
      [[nodiscard]] Guard lock();

   private:
      std::mutex mtx_; /**< Actual mutex that protects the object. */
      T          val_; /**< Object protected by the mutex. */
   };
} // namespace rw::thread

// IMPLEMENTATION
template<typename T>
template<typename... Args>
rw::thread::Mutex<T>::Mutex(Args&&... args) : val_{ std::forward<Args>(args)... }
{
}

template<typename T>
rw::thread::Mutex<T>::Guard rw::thread::Mutex<T>::lock()
{
   return Guard(std::scoped_lock(mtx_), val_);
}

#endif