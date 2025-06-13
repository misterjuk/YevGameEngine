#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

#include "IObserver.h"

namespace GameEvents
{
    static constexpr yev::IObserver::Event PlayerDied{ 0 };
    static constexpr yev::IObserver::Event PlayerDamaged{ 1 };
    static constexpr yev::IObserver::Event PlayerScored{ 2 };
    static constexpr yev::IObserver::Event EnemyKilled{ 3 };
    static constexpr yev::IObserver::Event LevelCompleted{ 4 };
    static constexpr yev::IObserver::Event TileDestroyed{ 5 };
    static constexpr yev::IObserver::Event ScoreChanged{ 6 };
}


#endif // GAME_EVENTS_H
