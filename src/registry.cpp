
#include "registry.hpp"

namespace recs
{
      Entity Registry::CreateEntity()
      {
         Entity entity = mMetaData.CreateEntity();
         entity.mRegistry = this;
         return entity;
      }

      void Registry::RemoveEntity(Entity entity)
      {

      }

      void Registry::RemoveComponet(Entity entity, std::string_view component_name)
      {

      }

      bool Registry::HasComponent(Entity entity, std::string_view component_name)
      {
         if (entity == iNVALID_ENTITY)
            throw std::runtime_error("Has component: invalid entity"); 
            
         auto component = mMetaData.CheckComponentPoolName(component_name);
         return mMetaData.HasComponent(entity, component);
      }

      Pool& Registry::GetComponentPool(ComponentTypeID id)
      {
         auto iterator = std::find_if(mComponentPools.begin(), mComponentPools.end(), 
                                       [id](const auto& id_pool){ return id_pool.first == id; });
         return iterator->second;
      }

}