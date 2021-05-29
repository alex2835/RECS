
#include "registry_meta.hpp"

namespace recs
{
   RegistryMeta::RegistryMeta()
      : mEntityCounter(1),
        mComponentCounter(1)
   {}

   Entity RegistryMeta::CreateEntity()
   {
      Entity entity(mEntityCounter++);
         
      std::vector<ComponentTypeID> components_ids;
      components_ids.reserve(5);

      mEntityComponentsMeta.emplace_back(entity, std::move(components_ids));
      return entity;
   }

   OptRef<std::vector<ComponentTypeID>>
   RegistryMeta::GetEntityComponents(Entity entity)
   {
      auto iterator = std::find_if(mEntityComponentsMeta.begin(), mEntityComponentsMeta.end(),
                                     [entity](const auto& entity_components){ return entity_components.first == entity; });
         
      if (iterator != mEntityComponentsMeta.end())
         return iterator->second;

      return std::nullopt;
   }

   bool RegistryMeta::HasComponent(Entity entity, ComponentTypeID component)
   {
      auto components = GetEntityComponents(entity);   
      return component ? HasComponent(*components, component) : false;
   }
      
   ComponentTypeID RegistryMeta::CheckComponentPoolName(std::string_view name)
   {
      auto iterator = std::find_if(mComponentNames.begin(), mComponentNames.end(), 
                                       [name](const auto& id_name){ return id_name.second == name; });
         
      return iterator != mComponentNames.end() ? iterator->first : INVALID_COMPONENT_TYPE;
   }

   ComponentTypeID RegistryMeta::AddComponentName(std::string_view name)
   {
      ComponentTypeID component = mComponentCounter++;
      mComponentNames.emplace_back(component, name);
      return component;
   }

   bool RegistryMeta::AddComponent(Entity entity, ComponentTypeID component)
   {
      auto components = GetEntityComponents(entity);
      if (components && !HasComponent(entity, component))
      {
         components->get().push_back(component);
         return true;
      }
      return false;
   }

   bool RegistryMeta::HasComponent(Ref<std::vector<ComponentTypeID>> components, ComponentTypeID component)
   {
      auto iterator = std::find(components->begin(), components->end(), component);
      return iterator != components->end();
   }

   void RegistryMeta::Remove(Entity entity)
   {
      auto iterator = std::find_if(mEntityComponentsMeta.begin(), mEntityComponentsMeta.end(),
                                    [=](const auto& entity_components){ return entity_components.first == entity; });
      if (iterator != mEntityComponentsMeta.end())
         mEntityComponentsMeta.erase(iterator);
   }

   void RegistryMeta::RemoveComponent(Entity entity, ComponentTypeID component)
   {
      auto iterator = std::find_if(mEntityComponentsMeta.begin(), mEntityComponentsMeta.end(),
                                    [=](const auto& entity_components){ return entity_components.first == entity; });
      
      if (iterator != mEntityComponentsMeta.end())
      {
         auto& components = iterator->second;
         auto component_iterator = std::find(components.begin(), components.end(), component);
         components.erase(component_iterator);
      }
   }

}