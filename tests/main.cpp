
#include "registry.hpp"
#include "registry_meta.hpp"
#include "entity.hpp"
#include "pool.hpp"
#include "view.hpp"

#include <cassert>
#include <iostream>
struct Speed
{
   float s;

   Speed(float speed)
    : s(speed)
   {}
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
         assert( std::string(e.what()) == "Has component: invalid entity");
      }
   }
      
   {
      recs::Entity entity = registry.CreateEntity();

      registry.AddComponet<Speed>(entity, "speed", 1.0f);
      registry.AddComponet<Position>(entity, "position", 0.0f, 0.0f);

      auto [speed, position] = registry.GetComponents<Speed, Position>(entity, { "speed", "position" } );

      speed.s += 1.0f;
      assert(speed == registry.GetComponent<Speed>(entity, "speed"));
   }

   {
      std::vector<recs::Entity> entities;
      for (int i = 0; i < 10; i++)
      {
         entities.push_back(registry.CreateEntity());
      }

      for (int i = 0; i < 10; i++)
      {
         recs::Entity entity = registry.CreateEntity();
         registry.AddComponet<Speed>(entity, "speed", i);
      }

      for (auto entity : entities)
      {
         registry.AddComponet<Speed>(entity, "speed", 5);
      }
   }

   {
      recs::Registry registry2;

      std::vector<recs::Entity> entities;
      for (int i = 0; i < 10; i++)
      {
         entities.push_back(registry2.CreateEntity());
      }
      for (int i = 0; i < 10; i++)
      {
         recs::Entity entity = registry2.CreateEntity();
         registry2.AddComponet<Speed>(entity, "speed", i);
      }

      int i = 0;
      for (auto entity : entities)
      {
         entity.AddComponet<Speed>("speed", 1.0f)
               .AddComponet<Position>("position", i++, 1.0f);
      }

      int count = 0;
      auto func = [&](Speed& speed, Position& position){ count++; };
      registry2.ForEach<Speed, Position>(func, { "speed", "position" } );
      assert(count == 10);

      count = 0;
      registry2.ForEach<Speed>([&](Speed& speed){ count++; }, { "speed" } );
      assert(count == 20);


   }

   return 0;
}