#pragma once
#include <unordered_map>
#include <string_view>
#include <string>

#include "entity.hpp"
#include "pool.hpp"
#include "view.hpp"
#include "registry_meta.hpp"

namespace recs
{
   class Registry
   {
   public:

      Entity CreateEntity() {}
      void   RemoveEntity(Entity entity);

      template <typename T>
      void AddComponet(Entity entity, std::string_view component_name)
      {
         ComponentType component = mMetaData.CheckComponentPoolName(component_name);
         if (component == INVALID_COMPONENT_TYPE)
         {
            component = mMetaData.AddComponentName(entity, component_name);
            mComponentPools.push_back( std::make_pair(component, Pool::MakePool<T>()) );
         }
      }

      View GetView(const std::initializer_list<std::string_view>& componets_names);

      const std::vector<Entity>& GetEntities();
      const RegistryMeta& GetMetaData();

   private:
      RegistryMeta mMetaData;
      std::vector<Entity> mEntities;
      std::vector<std::pair<ComponentType, Pool>> mComponentPools;
   };

}