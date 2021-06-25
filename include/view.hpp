#pragma once

#include "entity.hpp"
#include "recs_types.hpp"
namespace recs
{
   template<typename ...Args>
   class View
   {
   public:
      View( std::vector<Entity>&& entities,
            std::vector<std::tuple<Args&...>>&& components,
            const std::string_view(&component_names)[sizeof...(Args)])
         : mEntities(std::move(entities)),
           mComponents(std::move(components)),
           mComponentNames(component_names)
      {}

      template <typename F>
      void ForEach(F&& f)
      {
         for (auto& components : mComponents)
         {
            std::apply(f, components);
         }
      }

      std::tuple<Args&...> Get(Entity entity)
      {
         auto iterator = std::find(begin(), end(), entity);
         if (iterator == end())
            throw std::runtime_error("View::Get - Invalid entity");
         return *iterator;
      }

      std::tuple<Args&...> Get(size_t pos)
      {
         return mComponents[pos];
      }

      template <typename ...LocalArgs>
      std::tuple<LocalArgs&...> Get(std::array<ComponentTypeID, sizeof...(LocalArgs)> components)
      {
         
      }

      size_t Size() { return mEntities.size(); }
      
      auto begin() { return mEntities.begin(); }
      auto end() { return mEntities.end(); }
         
   private:
      std::vector<Entity> mEntities;
      std::vector<std::tuple<Args&...>> mComponents;
      const std::string_view(&mComponentNames)[sizeof...(Args)];
   };
   
}