#pragma once

#include "entity.hpp"
#include "recs_types.hpp"
namespace recs
{
   template<typename ...Args>
   class View
   {
   public:

      View(std::vector<Entity>&& entities, std::array<ComponentTypeID, sizeof...(Args)> components)
         : mEntities(std::move(entities)),
           mComponents(components)
      {}

      void ForEach();

      auto begin() { return mEntities.begin(); }
      auto end() { return mEntities.end(); }
         
   private:
      std::vector<Entity> mEntities;
      std::array<ComponentTypeID, sizeof...(Args)> mComponents;
   };
   
}