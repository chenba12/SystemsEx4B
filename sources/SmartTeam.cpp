#include "SmartTeam.hpp"

using namespace ariel;

SmartTeam::SmartTeam(Character *leader) : Team(leader) {}

//TODO implement
/**
 * does something
 * @param enemyTeam
 */
void SmartTeam::attack(Team *enemyTeam) {
    Team::attack(enemyTeam);
}

