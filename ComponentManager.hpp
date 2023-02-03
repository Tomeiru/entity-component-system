#pragma once

#include <unordered_map>
#include <memory>
#include "ComponentArray.hpp"
#include "Types.hpp"

namespace ECS {
    /**
     * @brief ComponentManager is a container for ComponentArrays
     */
    class ComponentManager {
    public:
        /**
         * @brief Register a new component type
         * @tparam ComponentT Type of the component to register
         */
        template<typename ComponentT>
        void registerComponent() {
            const char *type_name = typeid(ComponentT).name();

            if (type_name_to_component_type.find(type_name) != type_name_to_component_type.end())
                throw RuntimeException("ComponentManager::getComponentArray", "This Component Type has already been registered");
            type_name_to_component_type[type_name] = next_available_component_type++;
            type_name_to_component_array[type_name] = std::make_shared<ComponentArray<ComponentT>>();
        }
        /**
         * @brief Get the ComponentType of a component type
         * @tparam ComponentT Type of the component to get the ComponentType from
         * @return ComponentType of the component type
         */
        template<typename ComponentT>
        ComponentType getComponentType() {
            const char *type_name = typeid(ComponentT).name();

            if (type_name_to_component_type.find(type_name) == type_name_to_component_type.end())
                throw RuntimeException("ComponentManager::getComponentArray", "This Component Type has never been registered");
            return (type_name_to_component_type[type_name]);
        }
        /**
         * @brief Add a new component to an entity
         * @tparam ComponentT Type of the component to add
         * @param entity Entity to add the component to
         * @param component Component to add
         */
        template<typename ComponentT>
        void addComponent(Entity entity, ComponentT component) {
            getComponentArray<ComponentT>()->insertData(entity, component);
        }
        /**
         * @brief Remove a component from an entity
         * @tparam ComponentT Type of the component to remove
         * @param entity Entity to remove the component from
         */
        template<typename ComponentT>
        void removeComponent(Entity entity) {
            getComponentArray<ComponentT>()->removeData(entity);
        }
        /**
         * @brief Get the component of an entity
         * @tparam ComponentT Type of the component to get
         * @param entity Entity to get the component from
         * @return Component of the entity
         */
        template<typename ComponentT>
        ComponentT &getComponent(Entity entity) {
            return getComponentArray<ComponentT>()->getData(entity);
        }
        /**
         * @brief Check if an entity has a component
         * @tparam ComponentT Type of the component to check
         * @param entity Entity to check
         * @return true if the entity has the component, false otherwise
         */
        template<typename ComponentT>
        bool hasComponent(Entity entity) {
            return (getComponentArray<ComponentT>()->hasEntity(entity));
        }
        /**
         * @brief Destroy an entity
         * @param entity Entity to destroy
         */
        void entityDestroyed(Entity entity) {
            for (auto const &[_, component_array] : type_name_to_component_array)
                component_array->entityDestroyed(entity);
        }
    private:
        std::unordered_map<const char *, ComponentType> type_name_to_component_type;
        std::unordered_map<const char *, std::shared_ptr<IComponentArray>> type_name_to_component_array;
        ComponentType next_available_component_type;

        /**
         * @brief Get the ComponentArray of a component type
         * @tparam ComponentT Type of the component to get the ComponentArray from
         * @return ComponentArray of the component type
         */
        template<typename ComponentT>
        std::shared_ptr<ComponentArray<ComponentT>> getComponentArray()
        {
            const char *type_name = typeid(ComponentT).name();

            if (type_name_to_component_type.find(type_name) == type_name_to_component_type.end())
                throw RuntimeException("ComponentManager::getComponentArray", "This Component Type has never been registered");
            return std::static_pointer_cast<ComponentArray<ComponentT>>(type_name_to_component_array[type_name]);
        }

    };
}
