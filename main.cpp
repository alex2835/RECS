
#include "include/registry.hpp"
#include "include/registry_meta.hpp"
#include "include/entity.hpp"
#include "include/pool.hpp"
#include "include/view.hpp"

#include <cassert>
#include <iostream>
struct Speed
{
   float s;
};

int main(void)
{
   recs::Registry registry;

   recs::Entity entity = registry.CreateEntity();

   registry.AddComponet<Speed>(entity, "speed");
   assert(registry.HasComponent(entity, "speed"));
   assert(!registry.HasComponent(entity, "position"));

   try {
      registry.HasComponent(recs::iNVALID_ENTITY, "position");
   } catch (std::exception& e) {
      std::cout << e.what();
   }
   

   return 0;
}