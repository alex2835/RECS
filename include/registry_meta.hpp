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

      template <typename ...Args, size_t Size>
      View<Args...> GetView(const std::array<std::string_view, Size>& component_names);

      /**
       * @brief Return reference to componet name if exist
       * or invalid INVALID_COMPONENT_TYPE
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
   };


   // Implementation

   template <typename ...Args, size_t Size>
   View<Args...> RegistryMeta::GetView(const std::array<std::string_view, Size>& component_names)
   {
      std::array<ComponentTypeID, Size> components;
      for (int i = 0; i < Size; i++)
      {
         ComponentTypeID component_type = mMetaData.CheckComponentPoolName(component_names[i]);
         if (component_type)
            components[i] = &GetComponentPool(component_type);
         else
            throw std::runtime_error("ForEach: Ivalid component name " + std::string(component_names[i]));
      }
      std::vector<Entity> entities;
      entities.reserve(mEntityComponentsMeta.size() / 4;

      // Find entities with such components
      for (const auto& [entity, entity_componets] : mEntityComponentsMeta)
      {
         bool has_all = true; 
         for (auto entity_component : entity_componets)
         {
            bool has = false;
            for(auto component : components)
            {
               has |= entity_component == component;
            }
            has_all &= has;
         }
         if (has_all)
            entities.push_back(entity);
      }
      return View<Args...>(std::move(entities), components);
   }
   
}