#pragma once

#include "surface.h"

namespace Tmpl8
{
    class Gem
    {
    public:
        Gem();

        void Init();
        void Draw(Tmpl8::Surface* screen, int x, int y);
        bool CheckCollision(int x, int y, int playerWidth, int playerHeight);
        void Respawn(int index);
        int x[5], y[5];


    private:
        Sprite gemSprite;
    };
}