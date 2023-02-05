#pragma once

#include "Types.hpp"
#include <set>

namespace ECS {
/**
 * @brief System is a base class for all systems
 */
class System {
public:
    std::set<Entity> entities;
};
}
