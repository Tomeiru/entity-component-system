#pragma once

#include <memory>
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "ResourceManager.hpp"

namespace ECS {
    class Coordinator {
    public:
        Coordinator() : entity_manager(std::make_unique<EntityManager>()), component_manager(std::make_unique<ComponentManager>()), system_manager(std::make_unique<SystemManager>()), resource_manager(std::make_unique<ResourceManager>()) {};
        Entity createEntity() {
            return (entity_manager->createEntity());
        }
        void destroyEntity(Entity entity) {
            entity_manager->destroyEntity(entity);
            component_manager->entityDestroyed(entity);
            system_manager->entityDestroyed(entity);
        }
        template<typename ComponentT>
        void registerComponent() {
            component_manager->registerComponent<ComponentT>();
        }
        template<typename ComponentT>
        void addComponent(Entity entity, ComponentT component) {
            component_manager->addComponent(entity, component);
            auto signature = entity_manager->getSignature(entity);
            signature.set(component_manager->getComponentType<ComponentT>(), true);
            entity_manager->setSignature(entity, signature);
            system_manager->entitySignatureChanged(entity, signature);
        }
        template<typename ComponentT>
        void removeComponent(Entity entity) {
            component_manager->removeComponent<ComponentT>(entity);
            auto signature = entity_manager->getSignature(entity);
            signature.set(component_manager->getComponentType<ComponentT>(), false);
            entity_manager->setSignature(entity, signature);
            system_manager->entitySignatureChanged(entity, signature);
        }
        template<typename ComponentT>
        ComponentT &getComponent(Entity entity) {
            return component_manager->getComponent<ComponentT>(entity);
        }
        template<typename ComponentT>
        bool hasComponent(Entity entity) {
            return component_manager->hasComponent<ComponentT>(entity);
        }
        template<typename ComponentT>
        ComponentType getComponentType() {
            return component_manager->getComponentType<ComponentT>();
        }
        template<typename SystemT>
        std::shared_ptr<SystemT> registerSystem() {
            return system_manager->registerSystem<SystemT>();
        }
        template<typename SystemT>
        void setSignature(Signature signature) {
            system_manager->setSignature<SystemT>(signature);
        }
        template<typename SystemT, typename ComponentT>
        void setSignatureBit(bool value = true) {
            system_manager->setSignatureBit<SystemT>(getComponentType<ComponentT>(), value);
        }
        template<typename SystemT, typename ...ComponentTs>
        void setSignatureBits(bool value = true) {
            (system_manager->setSignatureBit<SystemT>(getComponentType<ComponentTs>(), value), ...);
        }
        template<typename ResourceT, typename ...Args>
        std::shared_ptr<ResourceT> registerResource(Args ...args) {
            return (resource_manager->registerResource<ResourceT>(args...));
        }
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
