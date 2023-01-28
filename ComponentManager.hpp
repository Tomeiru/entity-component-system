#pragma once

#include <unordered_map>
#include <memory>
#include "ComponentArray.hpp"
#include "Types.hpp"

namespace ECS {
    class ComponentManager {
    public:
        template<typename ComponentT>
        void registerComponent() {
            const char *type_name = typeid(ComponentT).name();

            if (type_name_to_component_type.find(type_name) != type_name_to_component_type.end())
                throw RuntimeException("ComponentManager::getComponentArray", "This Component Type has already been registered");
            type_name_to_component_type[type_name] = next_available_component_type++;
            type_name_to_component_array[type_name] = std::make_shared<ComponentArray<ComponentT>>();
        }
        template<typename ComponentT>
        ComponentType getComponentType() {
            const char *type_name = typeid(ComponentT).name();

            if (type_name_to_component_type.find(type_name) == type_name_to_component_type.end())
                throw RuntimeException("ComponentManager::getComponentArray", "This Component Type has never been registered");
            return (type_name_to_component_type[type_name]);
        }
        template<typename ComponentT>
        void addComponent(Entity entity, ComponentT component) {
            getComponentArray<ComponentT>()->insertData(entity, component);
        }
        template<typename ComponentT>
        void removeComponent(Entity entity) {
            getComponentArray<ComponentT>()->removeData(entity);
        }
        template<typename ComponentT>
        ComponentT &getComponent(Entity entity) {
            return getComponentArray<ComponentT>()->getData(entity);
        }
        template<typename ComponentT>
        bool hasComponent(Entity entity) {
            return (getComponentArray<ComponentT>()->hasEntity(entity));
        }
        void entityDestroyed(Entity entity) {
            for (auto const &[_, component_array] : type_name_to_component_array)
                component_array->entityDestroyed(entity);
        }
    private:
        std::unordered_map<const char *, ComponentType> type_name_to_component_type;
        std::unordered_map<const char *, std::shared_ptr<IComponentArray>> type_name_to_component_array;
        ComponentType next_available_component_type;

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
