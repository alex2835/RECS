#pragma once
#include <memory>
#include <mutex>
#include <tuple>
#include <vector>
#include <algorithm>

#include "recs_types.hpp"
#include "entity.hpp"

namespace recs
{
   /**
    * @brief Store sorted components data
    */
   class Pool
   {
   public:
      template <typename T>
      static Pool MakePool()
      {
         auto delete_func = [](void* component){ static_cast<T*>(component)->~T(); };
         auto copy_func = [](const void* from, void* to){  new (to) T(*static_cast<const T*>(from)); };
         return Pool(sizeof(T), delete_func, copy_func);
      }

      Pool(const Pool&);
      Pool& operator=(const Pool&) = delete;
      Pool(Pool&&) = default;
      Pool& operator=(Pool&&) = default;
      ~Pool();

      template <typename T, typename ...Args>
      T& Push(Entity entity, Args&& ...args);

      void Remove(Entity entity);

      template <typename T>
      T& Get(Entity entity);

      template <typename T>
      T& Get(size_t index);

      uint32_t Size() const noexcept { return mSize; }

      void* GetRaw(Entity entity);
      void* GetRaw(size_t index);

      Pool Clone() const;

   private:
      Pool() = default;
      Pool(uint32_t component_size, void(*delete_func)(void*), void(*copy_func)(const void*, void*));

      void Realloc(uint32_t new_capacity);
      void* GetElemAddress(uint32_t size);
      const void* GetElemAddressConst(uint32_t size) const;

   private:
      uint32_t mSize = 0;
      uint32_t mCapacity = 0;
      uint32_t mComponentSize = 0;
      std::unique_ptr<char[]> mData;
      std::vector<Entity> mEntities;

      void(*mDoDelete)(void* component);
      void(*mDoCopy)(const void* from, void* to);

      friend class Registry;
   };


    // Implementation

   template <typename T, typename ...Args>
   T& Pool::Push(Entity entity, Args&& ...args)
   {
      if (mCapacity <= mSize + 1)
         Realloc(2 * mSize + 1);

      auto iterator = std::lower_bound(mEntities.begin(), mEntities.end(), entity);
      int  position = iterator != mEntities.end() ? iterator - mEntities.begin() : mSize;

      mEntities.insert(iterator, entity);
      std::memmove(GetElemAddress(position + 1), GetElemAddress(position), mComponentSize * (mSize - position));
      void* new_elem_mem = GetElemAddress(position);
      new(new_elem_mem) T(std::forward<Args>(args)...);

      mSize++;
      auto& a = *static_cast<T*>(new_elem_mem);
      return a;
   }

   template <typename T>
   T& Pool::Get(Entity entity)
   {
      void* raw_data = GetRaw(entity);
      if (raw_data)
         return *static_cast<T*>(raw_data);
      throw std::runtime_error("Entity has component, but pool doesn't (Incorrent working)");
   }

   template <typename T>
   T& Pool::Get(size_t index)
   {
      if (index < Size())
         return *static_cast<T*>(GetElemAddress(index));
      throw std::runtime_error("Pool::Get, out of bound access");
   }

}