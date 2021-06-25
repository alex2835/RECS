
#include "pool.hpp"

namespace recs
{
      Pool::~Pool()
      {
         for (int i = 0; i < mSize; i++)
         {
            mDeliter(GetElemAddress(i));
         }
      }

      void Pool::Remove(Entity entity)
      {
         auto iterator = std::lower_bound(mEntities.begin(), mEntities.end(), entity);
         int  position = iterator != mEntities.end() ? iterator - mEntities.begin() : mSize;
         
         mEntities.erase(iterator);

         mDeliter(GetElemAddress(position));
         std::memmove(GetElemAddress(position), GetElemAddress(position + 1), mComponentSize * (mSize - position - 1));

         mSize--;
      }

      Pool::Pool(uint32_t component_size, void(*deliter)(void*))
         : mComponentSize(component_size),
           mSize(0),
           mCapacity(0),
           mDeliter(deliter)
      {
         Realloc(10);
      }

      void* Pool::GetRaw(Entity entity)
      {
         auto iterator = std::lower_bound(mEntities.begin(), mEntities.end(), entity);
         if (iterator != mEntities.end())
         {
            uint32_t position = iterator - mEntities.begin();
            return GetElemAddress(position);
         }
         return nullptr;
      }

      void Pool::Realloc(uint32_t new_capacity)
      {
         if (mCapacity < new_capacity)
         {
            char* new_data = new char[new_capacity * mComponentSize];
            memmove(new_data, mData.get(), mComponentSize * mSize);
            mData.reset(new_data);
            mCapacity = new_capacity;
         }
      }

      void* Pool::GetElemAddress(uint32_t size)
      {
         return &mData[mComponentSize * size];
      }
}