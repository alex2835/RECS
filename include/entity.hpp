#pragma once
#include <cstdint>

namespace recs
{
   class Registry;

   /**
    * @brief Entity is a reference to 
    * components that stored in pools
    */
   class Entity
   {
   public:
      Entity()  = default;
      ~Entity() = default;
      Entity(const Entity&) = default;
      Entity& operator = (const Entity&) = default;

      bool operator == (Entity other) const { return mID == other.mID; }
      bool operator <  (Entity other) const { return mID <  other.mID; }

   private:
      Entity(uint32_t id)
         : mID(id)
      {}
      
      Entity(uint32_t id, Registry* registry)
         : mID(id),
           mRegistry(registry)
      {}
   
   private:
      uint32_t  mID = 0u;
      Registry* mRegistry = nullptr;
      friend class RegistryMeta;
      friend class Registry;
   };

   constexpr Entity iNVALID_ENTITY = {};
}
