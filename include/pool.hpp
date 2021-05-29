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
         auto deliter = [](void* component){ static_cast<T*>(component)->~T(); };
         return Pool(sizeof(T), deliter);
      }

      Pool(Pool&&) = default;
      Pool& operator=(Pool&&) = default;

      template <typename T, typename ...Args>
      T& Push(Entity entity, Args&& ...args);

      void Remove(Entity entity);

      template <typename T>
      T& Get(Entity entity);

      uint32_t Size() const noexcept { return mSize; }

      void* GetRaw(Entity entity);

   private:
      Pool(uint32_t component_size, void(*deliter)(void*));

      Pool(const Pool&);
      Pool& operator=(const Pool&);

      void Realloc(uint32_t new_capacity);
      void* GetElemAddress(uint32_t size);

   private:
      uint32_t mComponentSize;
      uint32_t mSize;
      uint32_t mCapacity;
      std::unique_ptr<char[]> mData;
      std::vector<Entity> mEntities;
      void(*mDeliter)(void* component);

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
      return *static_cast<T*>(new_elem_mem);
   }

   template <typename T>
   T& Pool::Get(Entity entity)
   {
      void* raw_data = GetRaw(entity);
      if (raw_data)
         return *static_cast<T*>(raw_data);
      throw std::runtime_error("Entity has component, but pool doesn't (Incorrent working)");
   }

}