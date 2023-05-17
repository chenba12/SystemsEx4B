#include "Team2.hpp"

using namespace ariel;

Team2::Team2(Character *leader) : Team(leader) {}

/**
 * use the characters in this team to attack the other team
 * attack in order of when the character joined the team
 * @param enemyTeam
 * @throws runtime_error if the enemy team is already dead
 * @throws invalid_argument if entered a nullptr
 */
void Team2::attack(Team *enemyTeam) {
    if (enemyTeam == nullptr) {
        throw std::invalid_argument("got a nullptr");
    }
    if (enemyTeam->stillAlive() == 0) throw std::runtime_error("Team is dead");
    swapLeaderIfNeeded();
    Character *victim = findClosestTarget(enemyTeam);
    for (size_t i = 0; i < maxCharacters; ++i) {
        Character *member = getCharacters().at(i);
        if (member != nullptr) {
            victim = cowboyAttacking(enemyTeam, victim, member);
            victim = ninjaAttacking(enemyTeam, victim, member);
        }
    }
}

