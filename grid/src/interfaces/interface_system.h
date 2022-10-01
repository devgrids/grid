#pragma once

namespace grid
{
    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        ISystem() = default;

        virtual void start()
        {
        }

        virtual void update(const float& delta_time)
        {
        }

        virtual void render()
        {
        }
    };
}
