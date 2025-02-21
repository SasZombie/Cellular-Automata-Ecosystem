#pragma once

class Enviroment
{
public:
    virtual void info() = 0;
    virtual ~Enviroment() = default;
};

class Water : public Enviroment
{
public:
    void info();
};

