#pragma once

#include <set>
#include "Types.hpp"

namespace ECS {
    /**
     * @brief System is a base class for all systems
     */
    class System {
    public:
        std::set<Entity> entities;
    };
}
