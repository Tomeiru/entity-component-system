#pragma once

#include <queue>
#include <cassert>
#include <array>
#include <iostream>

#include "Types.hpp"
#include "RuntimeException.hpp"

namespace ECS {
    /**
     * @brief EntityManager is a container for entities
     */
    class EntityManager {
    public:
        /**
         * @brief Construct a new EntityManager object
         */
        EntityManager() {
            for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
                available_entities.push(entity);
        }
        /**
         * @brief Create a new entity
         * @return Entity created
         */
        Entity createEntity() {
            if (available_entities.empty())
                throw RuntimeException("EntityManager::createEntity", "Queue of available entity is empty, meaning that the Maximum number of entity has been reached");

            Entity entity = available_entities.front();
            available_entities.pop();
            return (entity);
        }
        /**
         * @brief Destroy an entity
         * @param entity Entity to destroy
         */
        void destroyEntity(Entity entity) {
            if (entity >= MAX_ENTITIES)
                throw RuntimeException("EntityManager::destroyEntity", "Entity passed as argument cannot exist (>= MAX_ENTITIES)");

            signatures[entity].reset();
            available_entities.push(entity);
        }
        /**
         * @brief Set the signature of an entity
         * @param entity Entity to set the signature of
         * @return true if the entity is alive, false otherwise
         */
        void setSignature(Entity entity, Signature signature) {
            if (entity >= MAX_ENTITIES)
                throw RuntimeException("EntityManager::destroyEntity", "Entity passed as argument cannot exist (>= MAX_ENTITIES)");

            signatures[entity] = signature;
        }
        /**
         * @brief Get the signature of an entity
         * @param entity Entity to get the signature of
         * @return true if the entity is alive, false otherwise
         */
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
