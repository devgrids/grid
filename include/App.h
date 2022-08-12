#pragma once

class App : public zar::IApplication
{
public:
     App();
    inline ~App() override;
    inline void start() override;
    inline void update() override;
    inline void render() override;
};
