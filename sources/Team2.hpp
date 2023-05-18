#ifndef SYSTEMSEX4A_TEAM2_H
#define SYSTEMSEX4A_TEAM2_H

#include "Team.hpp"

namespace ariel {
    class Team2 : public Team {
    public:
        explicit Team2(Character *leader);

        void attack(Team *enemyTeam) override;

        void print() override;

    };
}
#endif //SYSTEMSEX4A_TEAM2_H
