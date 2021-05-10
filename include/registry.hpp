#pragma once
#include <unordered_map>
#include <string_view>
#include <string>

#include "recs_types.hpp"
#include "entity.hpp"
#include "pool.hpp"
#include "view.hpp"
#include "registry_meta.hpp"

namespace recs
{
   class Registry
   {
   public:
      Entity CreateEntity() { return mMetaData.CreateEntity(); }
      void   RemoveEntity(Entity entity);

      template <typename T, typename ...Args>
      void AddComponet(Entity entity, std::string_view component_name, Args&& ...args)
      {
         if (entity == iNVALID_ENTITY)
            throw std::runtime_error("Add component: invalid entity");

         ComponentTypeID component_type = mMetaData.CheckComponentPoolName(component_name);
         if (component_type == INVALID_COMPONENT_TYPE)
         {
            component_type = mMetaData.AddComponentName(component_name);
            mComponentPools.push_back( std::make_pair(component_type, Pool::MakePool<T>()) );
         }
         mMetaData.AddComponent(entity, component_type);
         Pool& pool = GetComponentPool(component_type);
         pool.Push<T>(entity, std::forward<Args>(args)...);
      }

      void RemoveComponet(Entity entity, std::string_view component_name) {}

      template <typename T>
      OptRef<T> GetComponent(Entity entity, std::string_view component_name)
      {
         if (entity == iNVALID_ENTITY)
            throw std::runtime_error("Get component: invalid entity");

         ComponentTypeID component_type = mMetaData.CheckComponentPoolName(component_name);
         if (component_type)
         {
            Pool& pool = GetComponentPool(component_type);
            return pool.Get<T>(entity);
         }
         return std::nullopt;
      }

      bool HasComponent(Entity entity, std::string_view component_name)
      {
         if (entity == iNVALID_ENTITY)
            throw std::runtime_error("Has component: invalid entity");
            
         auto component = mMetaData.CheckComponentPoolName(component_name);
         return mMetaData.HasComponent(entity, component);
      }

      template <typename ...Args>
      View GetView(const std::initializer_list<std::string_view>& componets_names);

      const RegistryMeta& GetMetaData() { return mMetaData; };
      const auto& GetComponentPools() { return mComponentPools; };

   private:
      Pool& GetComponentPool(ComponentTypeID id)
      {
         auto iterator = std::find_if(mComponentPools.begin(), mComponentPools.end(), 
                                       [id](const auto& id_pool){ return id_pool.first == id; });
         return iterator->second;
      }

   private:
      RegistryMeta mMetaData;
      std::vector<std::pair<ComponentTypeID, Pool>> mComponentPools;
   };

}