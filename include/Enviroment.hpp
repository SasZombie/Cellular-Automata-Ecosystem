#pragma once
#include <cstddef>
#include "Entity.hpp"

namespace sas
{
    
    enum struct EnviromentType
    {
        WATER
    };

    class Enviroment : public Entity
    {
    public:
        size_t capacity = 100;
        virtual void info() const noexcept = 0;
        virtual bool hasCapacity(size_t consumption) const noexcept = 0;
        virtual ~Enviroment() = default;
    };

    class Water : public Enviroment
    {
    public:
        bool hasCapacity(size_t consumption) const noexcept;
        void info() const noexcept;
    };

} // namespace sas

// Water() = default;//Constructor
// Water(Water&& other) = delete;//Move constructor
// Water(const Water& other) = delete; //Copy constructor

// Water& operator=(const Water&& other) = delete;//Move operator
// Water& operator=(Water& other) = delete;//Copy Operator

// ~Water(); //Decnstructer