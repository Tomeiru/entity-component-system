#pragma once

#include <set>
#include "Types.hpp"

namespace ECS {
    /**
     * @brief System is a container for entities
     */
    class System {
    public:
        std::set<Entity> entities;
    };
}
