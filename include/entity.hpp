#pragma once
#include <cstdint>
#include <string_view>

namespace recs
{
   class Registry;

   /**
    * @brief Entity is a reference to 
    * components that stored in pools
    */
   class Entity
   {
   public:
      Entity()  = default;
      ~Entity() = default;
      Entity(const Entity&) = default;
      Entity& operator = (const Entity&) = default;

      bool operator == (Entity other) const { return mID == other.mID; }
      bool operator <  (Entity other) const { return mID <  other.mID; }

   template <typename T, typename ...Args>
   Entity AddComponet(std::string_view component_name, Args&& ...args)
   {
      mRegistry->template AddComponet<T>(*this, component_name, std::forward<Args>(args)...);
      return *this;
   }

   template <typename T>
   T& GetComponent(std::string_view component_name)
   {
      return mRegistry->template GetComponent<T>(component_name);
   }

   template <typename ...Args, int Size>
   std::tuple<Args&...> GetComponents(std::string_view const(&component_names)[Size])
   {
      return mRegistry->template GetComponents<Args...>(*this, component_names);
   }

   Entity RemoveComponet(std::string_view component_name);

   private:
      Entity(uint32_t id)
         : mID(id)
      {}
      
      Entity(uint32_t id, Registry* registry)
         : mID(id),
           mRegistry(registry)
      {}
   
   private:
      uint32_t  mID = 0u;
      Registry* mRegistry = nullptr;
      friend class RegistryMeta;
      friend class Registry;
   };

   constexpr Entity iNVALID_ENTITY = {};
}
