#pragma once
#include "Position.hpp"

namespace sas
{

    //Can now define strategies to Draw ---> No need to change plant just to draw something
    //We can alos overload to take different parameters
    class DrawStrategy
    {
    public:
        virtual void draw(const sas::Position &pos) const noexcept = 0;
        virtual ~DrawStrategy() = default;
    };

    class FlowerDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

    class TreeDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

    class WeedDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

    class PlaceholderDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

    class WaterDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

    class DesertDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

    class GrassDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

    class SnowDrawStrategy : public DrawStrategy
    {
    public:
        void draw(const sas::Position &pos) const noexcept override;
    };

} // namespace sas