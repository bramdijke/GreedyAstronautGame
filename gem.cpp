#include "gem.h"
#include "template.h"

namespace Tmpl8
{
    Gem::Gem() : gemSprite(new Surface("assets/gem.png"), 1) {}

    // Come up with 5 random locations for gems
    void Gem::Init()
    {
        for (int i = 0; i < 5; i++)
        {
            x[i] = IRand(800);
            y[i] = IRand(512);
        }
    }

    // Draw gem sprite
    void Gem::Draw(Tmpl8::Surface* screen, int x, int y)
    {
        gemSprite.Draw(screen, x, y);
    }

    // Check the collision of gems and player
    bool Gem::CheckCollision(int x, int y, int playerWidth, int playerHeight)
    {
        for (int i = 0; i < 5; i++)
        {
            if (x < this->x[i] + gemSprite.GetWidth() &&
                x + playerWidth > this->x[i] &&
                y < this->y[i] + gemSprite.GetHeight() &&
                y + playerHeight > this->y[i])
            {
                Respawn(i);
                return true;
            }
        }
        return false;
    }

    // If gem is interacted with, a new one respawns
    void Gem::Respawn(int index)
    {
        for (int j = index; j < 4; j++)
        {
            x[j] = x[j + 1];
            y[j] = y[j + 1];
        }
        x[4] = IRand(774); //minus gem image size
        y[4] = IRand(485); //minus gem image size
    }
}