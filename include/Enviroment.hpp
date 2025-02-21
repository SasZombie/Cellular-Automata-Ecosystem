#pragma once

class Enviroment
{
public:
    virtual void info() = 0;
};

class Water : public Enviroment
{
public:
    void info();
};

