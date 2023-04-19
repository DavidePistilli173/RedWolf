#ifndef RW_ALG_SORTING_HPP
#define RW_ALG_SORTING_HPP

#include <algorithm>
#include <functional>
#include <thread>

namespace rw::alg
{
   /**
    * @brief Sorts the last element in a container, with the assumption that all elements minus the last are sorted.
    * @tparam Iterator Iterator type.
    * @param first First element of the array to sort.
    * @param last Element next to the last element to sort.
    * @return Iterator pointing to the new position of the last element.
    */
   template<typename Iterator>
   constexpr Iterator sortLastElement(Iterator first, Iterator last);

   /**
    * @brief Sorts the last element in a container, with the assumption that all elements minus the last are sorted.
    * @tparam Iterator Iterator type.
    * @tparam Compare Comparator type.
    * @param first First element of the array to sort.
    * @param last Element next to the last element to sort.
    * @param comp Comparator object.
    * @return Iterator pointing to the new position of the last element.
    */
   template<typename Iterator, typename Compare>
   constexpr Iterator sortLastElement(Iterator first, Iterator last, Compare comp);
} // namespace rw::alg

// IMPLEMENTATION

template<typename Iterator>
constexpr Iterator rw::alg::sortLastElement(Iterator first, Iterator last)
{
   if (first == last) return last;
   if (last - first == 1) return first;

   --last;
   Iterator current = last - 1;
   while (current >= first && !(*current < *last))
   {
      std::swap(*current, *last);
      --current;
      --last;
   }
   return last;
}

template<typename Iterator, typename Compare>
constexpr Iterator rw::alg::sortLastElement(Iterator first, Iterator last, Compare comp)
{
   if (first == last) return last;
   if (last - first == 1) return first;

   --last;
   Iterator current = last - 1;
   while (current >= first && !comp(*current, *last))
   {
      std::swap(*current, *last);
      --current;
      --last;
   }
   return last;
}

#endif // SORTING_HPP
