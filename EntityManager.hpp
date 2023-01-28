#pragma once

#include <queue>
#include <cassert>
#include <array>
#include <iostream>

#include "Types.hpp"
#include "RuntimeException.hpp"

namespace ECS {

    class EntityManager {
    public:
        EntityManager() {
            for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
                available_entities.push(entity);
        }
        Entity createEntity() {
            if (available_entities.empty())
                throw RuntimeException("EntityManager::createEntity", "Queue of available entity is empty, meaning that the Maximum number of entity has been reached");

            Entity entity = available_entities.front();
            available_entities.pop();
            return (entity);
        }
        void destroyEntity(Entity entity) {
            if (entity >= MAX_ENTITIES)
                throw RuntimeException("EntityManager::destroyEntity", "Entity passed as argument cannot exist (>= MAX_ENTITIES)");

            signatures[entity].reset();
            available_entities.push(entity);
        }
        void setSignature(Entity entity, Signature signature) {
            if (entity >= MAX_ENTITIES)
                throw RuntimeException("EntityManager::destroyEntity", "Entity passed as argument cannot exist (>= MAX_ENTITIES)");

            signatures[entity] = signature;
        }
        Signature getSignature(Entity entity) {
            if (entity >= MAX_ENTITIES)
                throw RuntimeException("EntityManager::destroyEntity", "Entity passed as argument cannot exist (>= MAX_ENTITIES)");

            return (signatures[entity]);
        }
    private:
        std::array<Signature, MAX_ENTITIES> signatures;
        std::queue<Entity> available_entities;
    };

}
