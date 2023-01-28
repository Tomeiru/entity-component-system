#pragma once

#include <unordered_map>
#include <memory>
#include "System.hpp"
#include "Types.hpp"
#include "RuntimeException.hpp"

namespace ECS {
    class SystemManager {
    public:
        template <typename SystemT>
        std::shared_ptr<SystemT> registerSystem() {
            const char *type_name = typeid(SystemT).name();

            if (type_name_to_system.find(type_name) != type_name_to_system.end()) {
                throw RuntimeException("SystemManager::registerSystem", "This System Type has already been registered");
            }
            auto system = std::make_shared<SystemT>();
            type_name_to_system[type_name] = system;
            type_name_to_system_signature[type_name] = ECS::Signature();
            return system;
        }
        template <typename SystemT>
        void setSignature(Signature signature) {
            const char *type_name = typeid(SystemT).name();
            if (type_name_to_system.find(type_name) == type_name_to_system.end()) {
                throw RuntimeException("SystemManager::setSignature", "This System Type has not been registered yet");
            }
            type_name_to_system_signature[type_name] = signature;
        }
        template <typename SystemT>
        void setSignatureBit(size_t position, bool value = true) {
            const char *type_name = typeid(SystemT).name();
            if (type_name_to_system.find(type_name) == type_name_to_system.end()) {
                throw RuntimeException("SystemManager::setSignature", "This System Type has not been registered yet");
            }
            type_name_to_system_signature[type_name].set(position, value);
        }
        void entityDestroyed(Entity entity) {
            for (auto const &[_, system] : type_name_to_system)
                system->entities.erase(entity);
        }
        void entitySignatureChanged(Entity entity, Signature signature) {
            for (auto const &[type, system] : type_name_to_system) {
                auto const &system_signature = type_name_to_system_signature[type];

                if ((signature & system_signature) == system_signature)
                    system->entities.insert(entity);
                else
                    system->entities.erase(entity);
            }
        }
    private:
        std::unordered_map<const char *, Signature> type_name_to_system_signature;
        std::unordered_map<const char *, std::shared_ptr<System>> type_name_to_system;
    };
}
