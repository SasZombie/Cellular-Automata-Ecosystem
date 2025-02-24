#pragma once
#include <tuple>
#include <string>

namespace sas
{
    class Entity
    {
    public:
        std::pair<size_t, size_t> pos;
        std::pair<size_t, size_t> getPosition() const noexcept;
        // virtual std::string getType() const = 0;
        virtual ~Entity() = default;
    };
} // namespace sas