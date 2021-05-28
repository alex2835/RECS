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
    * @brief Store components data
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
      T& Push(Entity entity, Args&& ...args)
      {
         if (mCapacity <= mSize + 1)
            Realoc(2 * mSize + 1);

         void* new_elem_mem = GetElemAddress(mSize);
         
         mEntities.push_back(entity);
         new(new_elem_mem) T(std::forward<Args>(args)...);

         mSize++;
         return *static_cast<T*>(new_elem_mem);
      }

      void Remove(Entity entity)
      {
         
      }

      template <typename T>
      T& Get(Entity entity)
      {
         void* raw_data = GetRaw(entity);
         if (raw_data)
            return *static_cast<T*>(raw_data);
         throw std::runtime_error("Entity has component, but pool doesn't (Incorrent working)");
      }

      void* GetRaw(Entity entity)
      {
         auto iterator = std::find(mEntities.begin(), mEntities.end(), entity);
         if (iterator != mEntities.end())
         {
            uint32_t position = iterator - mEntities.begin();
            return GetElemAddress(position);
         }
         return nullptr;
      }

   private:
      Pool(uint32_t component_size, void(*deliter)(void*))
         : mComponentSize(component_size)
         , mSize(0)
         , mCapacity(0)
         , mDeliter(deliter)
      {
         Realoc(10);
      }

      Pool(const Pool&);
      Pool& operator=(const Pool&);

      void Realoc(uint32_t new_capacity)
      {
         if (mCapacity < new_capacity)
         {
            char* new_data = new char[new_capacity * mComponentSize];
            memmove(new_data, mData.get(), mComponentSize * mSize);
            mData.reset(new_data);
            mCapacity = new_capacity;
         }
      }

      void* GetElemAddress(uint32_t size)
      {
         return &mData[mComponentSize * size];
      }

   private:
      uint32_t mComponentSize;
      uint32_t mSize;
      uint32_t mCapacity;
      std::unique_ptr<char[]> mData;
      
      std::vector<Entity> mEntities;

      void(*mDeliter)(void* component);
    };

}