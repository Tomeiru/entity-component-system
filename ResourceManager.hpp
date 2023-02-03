#pragma once

#include <unordered_map>
#include <memory>
#include <unordered_map>
#include "RuntimeException.hpp"

namespace ECS {
    /**
     * @brief ResourceManager is a container for Resources
     */
    class ResourceManager {
    public:
        /**
         * @brief Register a new resource type
         * @tparam ResourceT Type of the resource to register
         * @tparam Args Types of the arguments to pass to the constructor of the resource
         * @param args Arguments to pass to the constructor of the resource
         * @return A shared pointer to the resource
         */
        template<typename ResourceT, typename ...Args>
        std::shared_ptr<ResourceT> registerResource(Args ...args) {
            const char *type_name = typeid(ResourceT).name();
            if (type_name_to_resource.find(type_name) != type_name_to_resource.end()) {
                throw RuntimeException("ResourceManager::registerResource", "This Resource Type has already been registered");
            }
            auto resource = std::make_shared<ResourceT>(args...);
            type_name_to_resource[type_name] = resource;
            return (resource);
        }
        /**
         * @brief Get a resource
         * @tparam ResourceT Type of the resource to get
         * @return A shared pointer to the resource
         */
        template<typename ResourceT>
        std::shared_ptr<ResourceT> getResource() {
            const char *type_name = typeid(ResourceT).name();
            auto find_result = type_name_to_resource.find(type_name);
            if (find_result == type_name_to_resource.end()) {
                throw RuntimeException("ResourceManager::getResource", "This Resource Type has not been registered yet");
            }
            return (std::static_pointer_cast<ResourceT>(find_result->second));
        }
    private:
        std::unordered_map<const char *, std::shared_ptr<void>> type_name_to_resource;
    };
}
