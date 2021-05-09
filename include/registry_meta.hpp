#pragma once
#include <vector>
#include <string>
#include <string_view>

#include "entity.hpp"

namespace recs
{
   typedef uint32_t ComponentType;
   constexpr ComponentType INVALID_COMPONENT_TYPE = 0;

   class RegistryMeta
   {
   public:

      /**
       * @brief Return reference to componet name if exist
       */
      ComponentType CheckComponentPoolName(std::string_view name) {}

      ComponentType AddComponentName(Entity entity, std::string_view name) {}
      
   private:
      std::vector<std::pair<Entity, std::vector<ComponentType>>> mEntityComponentsMeta;
      std::vector<std::pair<ComponentType, std::string>> mComponentNames;
   };
   
}