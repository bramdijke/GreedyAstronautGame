#pragma once
#include "surface.h"

namespace Tmpl8
{
    class Score {
    public:
        Score() { scoreValue = 0; }
        void IncrementScore() { scoreValue++; }
        int GetScore() { return scoreValue; }
        void Reset();

    private:
        int scoreValue;
    };
}