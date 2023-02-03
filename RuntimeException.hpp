#pragma once

#include <exception>
#include <string>

namespace ECS {
    /**
     * @brief RuntimeException is a custom exception for ECS
     */
    class RuntimeException : public std::exception {
    public:
        /**
         * @brief Construct a new RuntimeException object
         * @param where Where the exception was thrown
         * @param error_msg Error message
         */
        RuntimeException(const std::string &where, const std::string &error_msg) : error_msg(where+std::string(": ")+error_msg) {}
        /**
         * @brief Get the error message
         * @return const char* Error message
         */
        const char *what() const noexcept override {
            return (error_msg.c_str());
        }
    private:
        std::string error_msg;
    };
}