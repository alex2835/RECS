
#include "entity.hpp"
#include "registry.hpp"

namespace recs
{
   Entity Entity::RemoveComponet(std::string_view component_name)
   {
      mRegistry->RemoveComponet(*this, component_name);
      return *this;
   }
}