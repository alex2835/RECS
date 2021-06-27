
#include "registry.hpp"

namespace recs
{
      Entity Registry::CreateEntity()
      {
         Entity entity = mRegistryMeta.CreateEntity(this);
         return entity;
      }

      void Registry::RemoveEntity(Entity entity)
      {
         auto components = mRegistryMeta.GetEntityComponents(entity);
         if (components)
         {
            for (auto component_id : components->get())
            {
               auto iterator = std::find_if(mComponentPools.begin(), mComponentPools.end(),
                                                [=](const auto& id_pool){ return id_pool.first == component_id; });
               if (iterator != mComponentPools.end())
                  iterator->second.Remove(entity);
            }
            mRegistryMeta.Remove(entity);
         }
      }

      void Registry::RemoveComponet(Entity entity, std::string_view component_name)
      {
         ComponentTypeID component = mRegistryMeta.CheckComponentPoolName(component_name);
         if (mRegistryMeta.HasComponent(entity, component))
         {
            auto iterator = std::find_if(mComponentPools.begin(), mComponentPools.end(),
                                       [=](const auto& id_pool){ return id_pool.first == component; });
            iterator->second.Remove(entity);
            mRegistryMeta.RemoveComponent(entity, component);
         }
      }

      bool Registry::HasComponent(Entity entity, std::string_view component_name)
      {
         if (entity == iNVALID_ENTITY)
            throw std::runtime_error("Has component: invalid entity"); 
            
         auto component = mRegistryMeta.CheckComponentPoolName(component_name);
         return mRegistryMeta.HasComponent(entity, component);
      }

      Pool& Registry::GetComponentPool(ComponentTypeID id)
      {
         auto iterator = std::find_if(mComponentPools.begin(), mComponentPools.end(), 
                                       [id](const auto& id_pool){ return id_pool.first == id; });
         return iterator->second;
      }

      void Registry::CloneInto(Registry& registry)
      {
         registry.mRegistryMeta = mRegistryMeta;
         for (auto& [entity, components] : registry.mRegistryMeta.mEntityComponentsMeta)
            entity.mRegistry = &registry;

         for (const auto& [id, pool] : mComponentPools)
         {
            registry.mComponentPools.push_back(std::make_pair(id, pool.Clone()));
            for (auto& entity : registry.mComponentPools.back().second.mEntities)
               entity.mRegistry = &registry;
         }
      }

}