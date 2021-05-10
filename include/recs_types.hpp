#pragma once

#include <optional>
#include <type_traits>

namespace recs
{
   
   namespace detail
   {
      template <class T> constexpr T& FUN(T& t) noexcept { return t; }
      template <class T> void FUN(T&&) = delete;
   }

   template< class T >
   struct remove_cvref {
       typedef std::remove_cv_t<std::remove_reference_t<T>> type;
   };
   
   template <class T>
   class reference
   {
   public:
      // types
      typedef T type;
      
      // construct/copy/destroy
      template <class U, class = decltype(
         detail::FUN<T>(std::declval<U>()),
         std::enable_if_t<!std::is_same_v<reference, remove_cvref<U>>>()
      )>
      constexpr reference(U&& u) noexcept(noexcept(detail::FUN<T>(std::forward<U>(u))))
         : _ptr(std::addressof(detail::FUN<T>(std::forward<U>(u)))) {}
      reference(const reference&) noexcept = default;
      
      // assignment
      reference& operator=(const reference& x) noexcept = default;
      
      // access
      constexpr operator T& () const noexcept { return *_ptr; }
      constexpr T& get() const noexcept { return *_ptr; }
      constexpr T* operator->() const noexcept { return _ptr; }
      
      template< class... ArgTypes >
      constexpr std::invoke_result_t<T&, ArgTypes...>
         operator() ( ArgTypes&&... args ) const {
         return std::invoke(get(), std::forward<ArgTypes>(args)...);
      }
   
   private:
      T* _ptr;
   };
 
   // deduction guides
   template<class T>
   reference(T&) -> reference<T>;

   template <typename T>
   using OptRef = std::optional<reference<T>>;

   template <typename T>
   using Opt = std::optional<T>;

   template <typename T>
   using Ref = reference<T>;
}