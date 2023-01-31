#pragma once

#include <array>
#include <unordered_map>
#include "Types.hpp"
#include "RuntimeException.hpp"

namespace ECS {
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
    };

    template<typename ComponentT>
    class ComponentArray : public IComponentArray {
    public:
        ComponentArray() : index_last(0) {}
        void insertData(Entity entity, ComponentT component) {
            if (entity_to_index.find(entity) != entity_to_index.end())
                throw RuntimeException("ComponentArray::insertData", "Entity's component already in corresponding ComponentArray");
            entity_to_index[entity] = index_last;
            index_to_entity[index_last] = entity;
            component_array[index_last] = component;
            index_last++;
        }
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
        ComponentT &getData(Entity entity) {
            if (entity_to_index.find(entity) == entity_to_index.end())
                throw RuntimeException("ComponentArray::getData", "Entity's component is not contained in corresponding ComponentArray");
            return (component_array[entity_to_index[entity]]);
        }
        void entityDestroyed(Entity entity) override {
            if (entity_to_index.find(entity) != entity_to_index.end())
                removeData(entity);
        }
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
