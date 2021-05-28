
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

   Speed(float speed) : s(speed) {}
   bool operator==(const Speed& speed) const noexcept { return s == speed.s; } 
};

struct Position
{
   float x = 0.0f;
   float y = 0.0f;

   Position(float x, float y) : x(x), y(y) {}
   bool operator==(const Position& pos) const noexcept { return x == pos.x && y == pos.y; } 
};

int main(void)
{
   recs::Registry registry;

   {
      recs::Entity entity = registry.CreateEntity();

      registry.AddComponet<Speed>(entity, "speed", 0.0f);
      assert(registry.HasComponent(entity, "speed"));
      assert(!registry.HasComponent(entity, "position"));

      Speed& speed_component = registry.GetComponent<Speed>(entity, "speed");
      speed_component.s += 1.0f;
      assert(speed_component == registry.GetComponent<Speed>(entity, "speed"));

      try {
         registry.HasComponent(recs::iNVALID_ENTITY, "position");
      } catch (std::exception& e) {
         std::cout << e.what() << std::endl;
      }
   }
      
   {
      recs::Entity entity = registry.CreateEntity();

      registry.AddComponet<Speed>(entity, "speed", 0.0f);
      registry.AddComponet<Position>(entity, "position", 0.0f, 0.0f);

      auto [speed, position] = registry.GetComponents<Speed, Position>(entity, { "speed", "position" } );

      speed.s += 1.0f;
      assert(speed == registry.GetComponent<Speed>(entity, "speed"));
   }

   return 0;
}