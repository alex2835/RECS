
#include "include/registry.hpp"
#include "include/entity.hpp"
#include "include/pool.hpp"
#include "include/view.hpp"

struct Speed
{
   float s;
};

int main(void)
{
   recs::Registry registry;

   recs::Entity entity = registry.CreateEntity();

   registry.AddComponet<Speed>(entity, "speed");

   return 0;
}