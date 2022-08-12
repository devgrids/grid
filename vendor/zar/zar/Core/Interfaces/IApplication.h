#pragma once

namespace zar
{
    class __declspec(dllexport) IApplication
    {
    public:
        inline virtual ~IApplication() = default;
        inline IApplication() = default;

        inline virtual void start();
        inline virtual void update();
        inline virtual void render();

        int _width;
        int _height;
    };
}
