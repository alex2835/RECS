
#include "pool.hpp"

namespace recs
{
      Pool::Pool(uint32_t component_size, void(*delete_func)(void*), void(*copy_func)(const void*, void*))
         : mSize(0),
           mCapacity(0),
           mComponentSize(component_size),
           mDoDelete(delete_func),
           mDoCopy(copy_func)
      {
         Realloc(10);
      }
      
      Pool Pool::Clone() const
      {
         Pool pool;
         pool.mEntities = mEntities;
         pool.mDoDelete = mDoDelete;
         pool.mDoCopy   = mDoCopy;

         pool.mComponentSize = mComponentSize;
         pool.Realloc(mCapacity);
         pool.mSize = mSize;
         
         for (uint32_t i = 0; i < pool.mSize; i++)
            pool.mDoCopy(GetElemAddressConst(i), pool.GetElemAddress(i));

         return pool;
      }

      Pool::~Pool()
      {
         for (uint32_t i = 0; i < mSize; i++)
            mDoDelete(GetElemAddress(i));
      }

      void Pool::Remove(Entity entity)
      {
         auto iterator = std::lower_bound(mEntities.begin(), mEntities.end(), entity);
         int position = iterator != mEntities.end() ? iterator - mEntities.begin() : mSize;
         
         mEntities.erase(iterator);

         mDoDelete(GetElemAddress(position));
         std::memmove(GetElemAddress(position), GetElemAddress(position + 1), mComponentSize * (mSize - position - 1));

         mSize--;
      }

      void* Pool::GetRaw(Entity entity)
      {
         auto iterator = std::lower_bound(mEntities.begin(), mEntities.end(), entity);
         if (iterator != mEntities.end())
         {
            int position = iterator - mEntities.begin();
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

      const void* Pool::GetElemAddressConst(uint32_t size) const
      {
         return &mData[mComponentSize * size];
      }
}