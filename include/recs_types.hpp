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



   // ========== Meta programming ========== 

   template <typename ...Types>
   struct TypeList;

   template <typename H, typename ...T>
   struct TypeList<H, T...>
   {
      using Head = H;
      using Tail = TypeList<T...>;
   };

   template <>
   struct TypeList<> {};


   // Length
   template <typename TL>
   struct Length
   {
      static int const value = 1 + Length<typename TL::Tail>::value;
   };

   template <>
   struct Length<TypeList<>>
   {
      static int const value = 0;
   };

   // Cons
   template <typename H, typename TL>
   struct Cons;

   template <typename H, typename ...Types>
   struct Cons<H, TypeList<Types...>>
   {
      using type = TypeList<H, Types...>;
   };

   template <typename TL1, typename TL2>
   struct Concat;

   template <typename ...Ts1, typename ...Ts2>
   struct Concat<TypeList<Ts1...>, TypeList<Ts2...>>
   {
      using type = TypeList<Ts1..., Ts2...>;
   };

   template <size_t I,typename T> 
   struct tuple_n{
      template< typename...Args> using type = typename tuple_n<I-1, T>::template type<T, Args...>;
   };

   template <typename T> 
   struct tuple_n<0, T> {
      template<typename...Args> using type = std::tuple<Args...>;   
   };
   template <size_t I,typename T>  using tuple_of = typename tuple_n<I,T>::template type<>;

   template <typename T, size_t size>
   decltype(auto) make_tuple_n()
   {
      return tuple_of<size, T>();
   }

   template <typename T>
   decltype(auto) make_tuple_from_initializer_list(std::initializer_list<T> list)
   {
      return make_tuple_n<T, 2>();
   }

   template <typename T, size_t Size, size_t... Is>
   decltype(auto) as_tuple_impl(const T(&array)[Size], std::index_sequence<Is...>)
   {
      return std::make_tuple(array[Is]...);
   }

   template <typename T, size_t Size, typename Indices = std::make_index_sequence<Size>>
   decltype(auto) as_tuple(const T(&array)[Size])
   {
      return as_tuple_impl(array, Indices{});
   }

}