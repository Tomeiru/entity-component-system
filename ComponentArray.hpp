#pragma once

#include <array>
#include <unordered_map>
#include "Types.hpp"
#include "RuntimeException.hpp"

namespace ECS {
    /**
     * @brief Interface for ComponentArray
     */
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
    };

    /**
     * @brief ComponentArray is a container for components
     * @tparam ComponentT Type of the components
     */
    template<typename ComponentT>
    class ComponentArray : public IComponentArray {
    public:
        /**
         * @brief Construct a new ComponentArray object
         */
        ComponentArray() : index_last(0) {}
        /**
         * @brief Add a new component to an entity
         * @param entity Entity to add the component to
         * @param component Component to add
         */
        void insertData(Entity entity, ComponentT component) {
            if (entity_to_index.find(entity) != entity_to_index.end())
                throw RuntimeException("ComponentArray::insertData", "Entity's component already in corresponding ComponentArray");
            entity_to_index[entity] = index_last;
            index_to_entity[index_last] = entity;
            component_array[index_last] = component;
            index_last++;
        }
        /**
         * @brief Remove data from an entity
         * @param entity Entity to remove the data from
         */
        void removeData(Entity entity) {
            if (entity_to_index.find(entity) == entity_to_index.end())
                throw RuntimeException("ComponentArray::removeData", "Entity's component is not contained in corresponding ComponentArray");
            std::size_t index_to_delete = entity_to_index[entity];
            Entity replacing_entity = index_to_entity[index_last - 1];
            component_array[entity_to_index[entity]] = component_array[index_last - 1];
            entity_to_index[replacing_entity] = index_to_delete;
            index_to_entity[index_to_delete] = replacing_entity;
            entity_to_index.erase(entity);
            index_to_entity.erase(index_last - 1);
            index_last--;
        }
        /**
         * @brief Get the data of an entity
         * @param entity Entity to get the data from
         * @return ComponentT& Reference to the component
         */
        ComponentT &getData(Entity entity) {
            if (entity_to_index.find(entity) == entity_to_index.end())
                throw RuntimeException("ComponentArray::getData", "Entity's component is not contained in corresponding ComponentArray");
            return (component_array[entity_to_index[entity]]);
        }
        /**
         * @brief Remove data from a destroyed entity
         * @param entity Entity to remove the data from
         */
        void entityDestroyed(Entity entity) override {
            if (entity_to_index.find(entity) != entity_to_index.end())
                removeData(entity);
        }
        /**
         * @brief Check if the component array has an entity
         * @param entity Entity to check
         * @return true If the entity is in the component array, false otherwise
         */
        bool hasEntity(Entity entity) {
            return (entity_to_index.find(entity) != entity_to_index.end());
        }
    private:
        std::array<ComponentT, MAX_ENTITIES> component_array;
        std::unordered_map<Entity, std::size_t> entity_to_index;
        std::unordered_map<std::size_t, Entity> index_to_entity;
        std::size_t index_last;
    };
}
