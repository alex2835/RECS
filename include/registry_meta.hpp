#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <optional>
#include <string_view>
#include <array>

#include "recs_types.hpp"
#include "entity.hpp"
#include "view.hpp"

namespace recs
{

   /**
    * @brief 
    * 
    */
   class RegistryMeta
   {
   public:
      RegistryMeta();

      Entity CreateEntity();

      OptRef<std::vector<ComponentTypeID>> GetEntityComponents(Entity entity);

      bool HasComponent(Entity entity, ComponentTypeID component);

      template <size_t Size>
      bool HasComponents(Entity entity, const std::array<ComponentTypeID, Size>& components);

      void Remove(Entity entity);

      void RemoveComponent(Entity entity, ComponentTypeID component);

      /**
       * @brief Return index of componet name if exist
       * or INVALID_COMPONENT_TYPE
       */
      ComponentTypeID CheckComponentPoolName(std::string_view name);

      ComponentTypeID AddComponentName(std::string_view name);

      bool AddComponent(Entity entity, ComponentTypeID component);

   private:
      bool HasComponent(Ref<std::vector<ComponentTypeID>> components, ComponentTypeID component);
      
   private:
      uint32_t mEntityCounter;
      uint32_t mComponentCounter;
      std::vector<std::pair<Entity, std::vector<ComponentTypeID>>> mEntityComponentsMeta;
      std::vector<std::pair<ComponentTypeID, std::string>> mComponentNames;

      friend class Registry;
   };
   
}