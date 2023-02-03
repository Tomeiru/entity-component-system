#pragma once

#include <memory>
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "ResourceManager.hpp"

namespace ECS {
    /**
     * @brief Coordinator is the main class of the ECS
     */
    class Coordinator {
    public:
        /**
         * @brief Construct a new Coordinator object
         */
        Coordinator() : entity_manager(std::make_unique<EntityManager>()), component_manager(std::make_unique<ComponentManager>()), system_manager(std::make_unique<SystemManager>()), resource_manager(std::make_unique<ResourceManager>()) {};
        /**
         * @brief Create a new entity
         * @return Entity created
         */
        Entity createEntity() {
            return (entity_manager->createEntity());
        }
        /**
         * @brief Destroy an entity and alert the managers
         * @param entity Entity to destroy
         */
        void destroyEntity(Entity entity) {
            entity_manager->destroyEntity(entity);
            component_manager->entityDestroyed(entity);
            system_manager->entityDestroyed(entity);
        }
        /**
         * @brief Register a component type to the component manager
         * @tparam ComponentT Type of the component to register
         */
        template<typename ComponentT>
        void registerComponent() {
            component_manager->registerComponent<ComponentT>();
        }
        /**
         * @brief Add a component to an entity
         * @tparam ComponentT Type of the component to add
         * @param entity Entity to add the component to
         * @param component Component to add
         */
        template<typename ComponentT>
        void addComponent(Entity entity, ComponentT component) {
            component_manager->addComponent(entity, component);
            auto signature = entity_manager->getSignature(entity);
            signature.set(component_manager->getComponentType<ComponentT>(), true);
            entity_manager->setSignature(entity, signature);
            system_manager->entitySignatureChanged(entity, signature);
        }
        /**
         * @brief Remove a component from an entity
         * @tparam ComponentT Type of the component to remove
         * @param entity Entity to remove the component from
         */
        template<typename ComponentT>
        void removeComponent(Entity entity) {
            component_manager->removeComponent<ComponentT>(entity);
            auto signature = entity_manager->getSignature(entity);
            signature.set(component_manager->getComponentType<ComponentT>(), false);
            entity_manager->setSignature(entity, signature);
            system_manager->entitySignatureChanged(entity, signature);
        }
        /**
         * @brief Get a component from an entity
         * @tparam ComponentT Type of the component to get
         * @param entity Entity to get the component from
         * @return ComponentT& Reference to the component
         */
        template<typename ComponentT>
        ComponentT &getComponent(Entity entity) {
            return component_manager->getComponent<ComponentT>(entity);
        }
        /**
         * @brief Check if an entity has a component
         * @tparam ComponentT Type of the component to check
         * @param entity Entity to check
         * @return true If the entity has the component, false otherwise
         */
        template<typename ComponentT>
        bool hasComponent(Entity entity) {
            return component_manager->hasComponent<ComponentT>(entity);
        }
        /**
         * @brief Get the Component Type object
         * @tparam ComponentT Type of the component to get the type from
         * @return ComponentType Type of the component
         */
        template<typename ComponentT>
        ComponentType getComponentType() {
            return component_manager->getComponentType<ComponentT>();
        }
        /**
         * @brief Register a system
         * @tparam SystemT Type of the system to register
         * @return std::shared_ptr<SystemT> Pointer to the system
         */
        template<typename SystemT>
        std::shared_ptr<SystemT> registerSystem() {
            return system_manager->registerSystem<SystemT>();
        }
        /**
         * @brief Set the signature of a system
         * @tparam SystemT Type of the system to set the signature of
         * @param signature Signature to set
         */
        template<typename SystemT>
        void setSignature(Signature signature) {
            system_manager->setSignature<SystemT>(signature);
        }
        /**
         * @brief Set the signature bit of a system
         * @tparam SystemT Type of the system to set the signature bit of
         * @tparam ComponentT Type of the component to set the signature bit of
         * @param value Value to set the signature bit to
         */
        template<typename SystemT, typename ComponentT>
        void setSignatureBit(bool value = true) {
            system_manager->setSignatureBit<SystemT>(getComponentType<ComponentT>(), value);
        }
        /**
         * @brief Set the signature bits of a system
         * @tparam SystemT Type of the system to set the signature bits of
         * @tparam ComponentTs Types of the components to set the signature bits of
         * @param value Value to set the signature bits to
         */
        template<typename SystemT, typename ...ComponentTs>
        void setSignatureBits(bool value = true) {
            (system_manager->setSignatureBit<SystemT>(getComponentType<ComponentTs>(), value), ...);
        }
        /**
         * @brief Register a resource
         * @tparam SystemT Type of the resource to register
         * @tparam Args Types of the arguments to pass to the resource constructor
         * @param args Arguments to pass to the resource constructor
         * @return std::shared_ptr<ResourceT> Pointer to the resource
         */
        template<typename ResourceT, typename ...Args>
        std::shared_ptr<ResourceT> registerResource(Args ...args) {
            return (resource_manager->registerResource<ResourceT>(args...));
        }
        /**
         * @brief Get a resource
         * @tparam ResourceT Type of the resource to get
         * @return std::shared_ptr<ResourceT> Pointer to the resource
         */
        template<typename ResourceT>
        std::shared_ptr<ResourceT> getResource() {
            return (resource_manager->getResource<ResourceT>());
        }
    private:
        std::unique_ptr<EntityManager> entity_manager;
        std::unique_ptr<ComponentManager> component_manager;
        std::unique_ptr<SystemManager> system_manager;
        std::unique_ptr<ResourceManager> resource_manager;
    };
}
