#pragma once

#include <set>
#include "Types.hpp"

namespace ECS {
    class System {
    public:
        std::set<Entity> entities;
    };
}
