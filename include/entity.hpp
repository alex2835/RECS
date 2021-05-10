#pragma once
#include <cstdint>

namespace recs
{
   /**
    * @brief Entity is a reference to 
    * components that stored in pools
    */
   class Entity
   {
   public:
      Entity() = default;
      ~Entity() = default;
      Entity(const Entity&) = default;
      Entity& operator=(const Entity&) = default;

      bool operator == (Entity other) const { return mID == other.mID; }

   private:
      Entity(uint32_t id) 
         : mID(id)
      {}
   
   private:
      uint32_t mID = 0u;
      friend class RegistryMeta;
   };

   constexpr Entity iNVALID_ENTITY = {};
}
