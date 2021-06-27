#pragma once
#include <unordered_map>
#include <string_view>
#include <string>
#include <tuple>

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
      Registry() = default;
      Registry(const Registry&) = delete;
      Registry& operator=(const Registry&) = delete;
      Registry(Registry&&) = delete;
      Registry& operator=(Registry&&) = delete;

      Entity CreateEntity();

      void RemoveEntity(Entity entity);

      template <typename T, typename ...Args>
      void AddComponet(Entity entity, std::string_view component_name, Args&& ...args);

      void RemoveComponet(Entity entity, std::string_view component_name);

      template <typename T>
      T& GetComponent(Entity entity, std::string_view component_name);

      template <typename ...Args, int Size>
      std::tuple<Args&...> GetComponents(Entity entity, const std::string_view(&component_names)[Size]);

      bool HasComponent(Entity entity, std::string_view component_name);

      template <size_t Size>
      bool HasComponents(Entity entity, const std::string_view(&component_names)[Size]);

      template <typename ...Args, typename F, size_t Size>
      void ForEach(F&& func, const std::string_view(&component_names)[Size]);

      template <typename ...Args, size_t Size>
      View<Args...> GetView(const std::string_view(&component_names)[Size]);

      void CloneInto(Registry&);

   private:
      template <typename ...Args, typename Tuple, size_t ...Is>
      std::tuple<Args&...> GetComponentsImpl(Entity entity, Tuple tuple, std::index_sequence<Is...>)
      {
         return std::forward_as_tuple(GetComponent<Args>(entity, std::get<Is>(tuple))...);
      }

      template <typename ...Args, size_t Size, size_t ...Is>
      std::tuple<Args&...> MakeTupleFromPoolsAndIndicies(Pool* const(&pools)[Size], size_t const(&indicies)[Size], std::index_sequence<Is...>)
      {
         return std::forward_as_tuple(pools[Is]->template Get<Args>(indicies[Is])...);
      }

      Pool& GetComponentPool(ComponentTypeID id);

   private:
      RegistryMeta mRegistryMeta;
      std::vector<std::pair<ComponentTypeID, Pool>> mComponentPools;
   };


   // Implemantation

   template <typename T, typename ...Args>
   void Registry::AddComponet(Entity entity, std::string_view component_name, Args&& ...args)
   {
      if (entity == iNVALID_ENTITY)
         throw std::runtime_error("Add component: invalid entity");

      ComponentTypeID component_type = mRegistryMeta.CheckComponentPoolName(component_name);
      if (component_type == INVALID_COMPONENT_TYPE)
      {
         component_type = mRegistryMeta.AddComponentName(component_name);
         mComponentPools.push_back(std::make_pair(component_type, Pool::MakePool<T>()));
      }
      mRegistryMeta.AddComponent(entity, component_type);
      Pool& pool = GetComponentPool(component_type);
      pool.Push<T>(entity, std::forward<Args>(args)...);
   }

   template <typename T>
   T& Registry::GetComponent(Entity entity, std::string_view component_name)
   {
      if (entity == iNVALID_ENTITY)
         throw std::runtime_error("Get component: invalid entity");

      ComponentTypeID component_type = mRegistryMeta.CheckComponentPoolName(component_name);
      if (component_type)
      {
         Pool& pool = GetComponentPool(component_type);
         return pool.Get<T>(entity);
      }
      throw std::runtime_error("Entity doesn't have such a component " + std::string(component_name));
   }

   template <typename ...Args, int Size>
   std::tuple<Args&...> Registry::GetComponents(Entity entity, std::string_view const(&component_names)[Size])
   {
      static_assert(sizeof...(Args) == Size, "Component names and template arguments are not equal");

      if (entity == iNVALID_ENTITY)
         throw std::runtime_error("Get component: invalid entity");

      return GetComponentsImpl<Args...>(entity, as_tuple(component_names), std::make_index_sequence<Size>{});
   }

   template <size_t Size>
   bool Registry::HasComponents(Entity entity, const std::string_view(&component_names)[Size])
   {
      if (entity == iNVALID_ENTITY)
         throw std::runtime_error("Has components: invalid entity"); 

      int i = 0;
      std::array<ComponentTypeID, Size> components;

      for (auto component_name : component_names)
      {
         auto component = mRegistryMeta.CheckComponentPoolName(component_name);
         if (component)
            components[i++] = component;
         else
            throw std::runtime_error("Has components: invalid component name " + std::string(component_name));
      }
      return mRegistryMeta.HasComponents(entity, components);
   }

   template <typename ...Args, size_t Size>
   View<Args...> Registry::GetView(const std::string_view(&component_names)[Size])
   {
      static_assert(sizeof...(Args) == Size, "Component names and template arguments are not equal");

      Pool* pools[Size];
      std::vector<Entity> entities;
      std::vector<std::tuple<Args&...>> components;
      entities.reserve(mRegistryMeta.mEntityComponentsMeta.size() / 4);

      for (int i = 0; i < Size; i++)
      {
         ComponentTypeID component_type = mRegistryMeta.CheckComponentPoolName(component_names[i]);
         if (component_type)
            pools[i] = &GetComponentPool(component_type);
         else
            throw std::runtime_error("ForEach: Ivalid component name " + std::string(component_names[i]));
      }

      uint32_t max_id = 0;
      size_t indicies[Size] = { 0u };
      while (true)
      {
         for (int i = 0; i < Size; i++)
         {
            if (indicies[i] >= pools[i]->Size())
               goto end;

            if (max_id < pools[i]->mEntities[indicies[i]].mID)
               max_id = pools[i]->mEntities[indicies[i]].mID;
         }

         bool next = false;
         for (int i = 0; i < Size; i++)
         {
            while (pools[i]->mEntities[indicies[i]].mID < max_id)
            {
               if (pools[i]->mEntities[indicies[i]].mID > max_id)
               {
                  next = true;
                  break;
               }
               
               indicies[i]++;
               if (indicies[i] >= pools[i]->Size())
                  goto end;
            }
         }
         if (next) continue;

         entities.push_back(Entity(max_id));
         
         auto tuple = MakeTupleFromPoolsAndIndicies<Args...>(pools, indicies, std::make_index_sequence<Size>{});
         components.push_back(tuple);

         for (int i = 0; i < Size; i++)
            indicies[i]++;
      }
      end:
      return View<Args...>(std::move(entities), std::move(components), component_names);
   }

   template <typename ...Args, typename F, size_t Size>
   void Registry::ForEach(F&& func, const std::string_view(&component_names)[Size])
   {
      static_assert(sizeof...(Args) == Size, "Component names and template arguments are not equal");

      Pool* pools[Size];
      for (int i = 0; i < Size; i++)
      {
         ComponentTypeID component_type = mRegistryMeta.CheckComponentPoolName(component_names[i]);
         if (component_type)
            pools[i] = &GetComponentPool(component_type);
         else
            throw std::runtime_error("ForEach: Ivalid component name " + std::string(component_names[i]));
      }

      uint32_t max_id = 0;
      size_t indicies[Size] = { 0u };
      while (true)
      {
         for (int i = 0; i < Size; i++)
         {
            if (indicies[i] >= pools[i]->Size())
               return;

            if (max_id < pools[i]->mEntities[indicies[i]].mID)
               max_id = pools[i]->mEntities[indicies[i]].mID;
         }

         bool next = false;
         for (int i = 0; i < Size; i++)
         {
            while (pools[i]->mEntities[indicies[i]].mID < max_id)
            {
               if (pools[i]->mEntities[indicies[i]].mID > max_id)
               {
                  next = true;
                  break;
               }
               
               indicies[i]++;
               if (indicies[i] >= pools[i]->Size())
                  return;
            }
         }
         if (next) continue;

         auto tuple = MakeTupleFromPoolsAndIndicies<Args...>(pools, indicies, std::make_index_sequence<Size>{});
         std::apply(std::forward<F>(func), tuple);

         for (int i = 0; i < Size; i++)
            indicies[i]++;
      }

   }

}