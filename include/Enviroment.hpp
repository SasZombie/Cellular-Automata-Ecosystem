#pragma once

namespace sas
{
    

class Enviroment
{
public:
    virtual void info() = 0;
    virtual ~Enviroment() = default;
};

class Water : public Enviroment
{
public:
    size_t waterCapacity;

    // Water() = default;//Constructor
    // Water(Water&& other) = delete;//Move constructor
    // Water(const Water& other) = delete; //Copy constructor

    // Water& operator=(const Water&& other) = delete;//Move operator
    // Water& operator=(Water& other) = delete;//Copy Operator

    // ~Water(); //Decnstructer
    void info();
};


} // namespace sas