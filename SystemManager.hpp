#pragma once

#include "RuntimeException.hpp"
#include "System.hpp"
#include "Types.hpp"
#include <memory>
#include <unordered_map>

namespace ECS {
/**
 * @brief SystemManager is a container for Systems
 */
class SystemManager {
public:
    /**
     * @brief Register a new system type
     * @tparam SystemT Type of the system to register
     * @return A shared pointer to the system
     */
    template<typename SystemT>
    std::shared_ptr<SystemT> registerSystem()
    {
        const char* type_name = typeid(SystemT).name();

        if (type_name_to_system.find(type_name) != type_name_to_system.end()) {
            throw RuntimeException("SystemManager::registerSystem", "This System Type has already been registered");
        }
        auto system = std::make_shared<SystemT>();
        type_name_to_system[type_name] = system;
        type_name_to_system_signature[type_name] = ECS::Signature();
        return system;
    }
    /**
     * @brief set the signature of a system
     * @tparam SystemT Type of the system to set the signature of
     * @param signature The signature to set
     */
    template<typename SystemT>
    void setSignature(Signature signature)
    {
        const char* type_name = typeid(SystemT).name();
        if (type_name_to_system.find(type_name) == type_name_to_system.end()) {
            throw RuntimeException("SystemManager::setSignature", "This System Type has not been registered yet");
        }
        type_name_to_system_signature[type_name] = signature;
    }
    /**
     * @brief set a bit of the signature of a system
     * @tparam SystemT Type of the system to set the signature of
     * @param position The position of the bit to set
     * @param value The value to set the bit to
     */
    template<typename SystemT>
    void setSignatureBit(size_t position, bool value = true)
    {
        const char* type_name = typeid(SystemT).name();
        if (type_name_to_system.find(type_name) == type_name_to_system.end()) {
            throw RuntimeException("SystemManager::setSignature", "This System Type has not been registered yet");
        }
        type_name_to_system_signature[type_name].set(position, value);
    }
    /**
     * @brief handle the destruction of an entity by removing it from all systems
     * @param entity The entity that has been destroyed
     */
    void entityDestroyed(Entity entity)
    {
        for (auto const& [_, system] : type_name_to_system)
            system->entities.erase(entity);
    }
    /**
     * @brief handle the signature change of an entity
     * @param entity The entity that has changed
     * @param signature The new signature of the entity
     */
    void entitySignatureChanged(Entity entity, Signature signature)
    {
        for (auto const& [type, system] : type_name_to_system) {
            auto const& system_signature = type_name_to_system_signature[type];

            if ((signature & system_signature) == system_signature)
                system->entities.insert(entity);
            else
                system->entities.erase(entity);
        }
    }
    /**
     * @brief Get a resource
     * @tparam SystemT Type of the system to get
     * @return A shared pointer to the system
     */
    template<typename SystemT>
    std::shared_ptr<SystemT> getSystem()
    {
        const char* type_name = typeid(SystemT).name();
        auto find_result = type_name_to_system.find(type_name);
        if (find_result == type_name_to_system.end()) {
            throw RuntimeException("SystemManager::getSystem", "This Resource Type has not been registered yet");
        }
        return (std::static_pointer_cast<SystemT>(find_result->second));
    }

private:
    std::unordered_map<const char*, Signature> type_name_to_system_signature;
    std::unordered_map<const char*, std::shared_ptr<System>> type_name_to_system;
};
}
