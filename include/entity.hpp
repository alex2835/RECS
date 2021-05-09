#pragma once
#include <cstdint>

namespace recs
{
   constexpr uint32_t iNVALID_ENTITY = 0;

   /**
    * @brief Entity is a reference to 
    * components that stored in pools
    * 
    */
   class Entity
   {
   public:
      //Entity() = default;
      //~Entity() = default;

      bool operator == (Entity other) { return id = other.id; }

   private:
      uint32_t id;
   };
}