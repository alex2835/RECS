#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <optional>
#include <string_view>

#include "recs_types.hpp"
#include "entity.hpp"

namespace recs
{
   typedef uint32_t ComponentTypeID;
   constexpr ComponentTypeID INVALID_COMPONENT_TYPE = 0;

   class RegistryMeta
   {
   public:

      RegistryMeta()
         : mEntityCounter(1)
         , mComponentCounter(1)
      {}

      Entity CreateEntity()
      {
         Entity entity(mEntityCounter++);
         
         std::vector<ComponentTypeID> components_ids;
         components_ids.reserve(5);

         mEntityComponentsMeta.emplace_back(entity, std::move(components_ids));
         return entity;
      }

      OptRef<std::vector<ComponentTypeID>>
      GetEntityComponents(Entity entity)
      {
         auto iterator = std::find_if(mEntityComponentsMeta.begin(), mEntityComponentsMeta.end(),
                                        [entity](const auto& entity_components){ return entity_components.first == entity; });
         
         if (iterator != mEntityComponentsMeta.end())
            return iterator->second;

         return std::nullopt;
      }

      bool HasComponent(Entity entity, ComponentTypeID component)
      {
         auto components = GetEntityComponents(entity);   
         return component ? HasComponent(*components, component) : false;
      }

      /**
       * @brief Return reference to componet name if exist
       */
      ComponentTypeID CheckComponentPoolName(std::string_view name)
      {
         auto iterator = std::find_if(mComponentNames.begin(), mComponentNames.end(), 
                                          [name](const auto& id_name){ return id_name.second == name; });
         
         return iterator != mComponentNames.end() ? iterator->first : INVALID_COMPONENT_TYPE;
      }

      ComponentTypeID AddComponentName(std::string_view name)
      {
         ComponentTypeID component = mComponentCounter++;
         mComponentNames.emplace_back(component, name);
         return component;
      }

      bool AddComponent(Entity entity, ComponentTypeID component)
      {
         auto components = GetEntityComponents(entity);
         if (components && !HasComponent(entity, component))
         {
            components->get().push_back(component);
            return true;
         }
         return false;
      }

   private:
      bool HasComponent(Ref<std::vector<ComponentTypeID>> components, ComponentTypeID component)
      {
         auto iterator = std::find(components->begin(), components->end(), component);
         return iterator != components->end();
      }
      
   private:
      uint32_t mEntityCounter;
      uint32_t mComponentCounter;
      std::vector<std::pair<Entity, std::vector<ComponentTypeID>>> mEntityComponentsMeta;
      std::vector<std::pair<ComponentTypeID, std::string>> mComponentNames;
   };
   
}