#ifndef SYSTEMSEX4A_SMARTTEAM_H
#define SYSTEMSEX4A_SMARTTEAM_H

#include <vector>
#include "Team.hpp"

namespace ariel {
    class SmartTeam : public Team {
    private:
        std::vector<Character *> targetedEnemies;
    public:
        explicit SmartTeam(Character *leader);

        void attack(Team *enemyTeam) override;

        void print() override;

    private:
        static double enemyScore(Character *enemy, Team *enemyTeam, Character *attackingCharacter);
    };
}

#endif
