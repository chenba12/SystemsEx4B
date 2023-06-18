#include "SmartTeam.hpp"
#include <map>

using namespace ariel;


// This function calculates and returns a score for a given enemy.
// The score is calculated based on several factors:
// 1. The enemy's health - lower health enemies have a higher score.
// 2. The type of the character making the attack - different types of characters (Cowboy or different types of Ninjas) adjust the score differently.
// 3. The number of bullets left (if the character is a Cowboy) - the more bullets left, the higher the score.
// 4. The proximity of other enemies - the more enemies are near the target enemy, the higher the score.
double SmartTeam::enemyScore(Character *enemy, Team *enemyTeam, Character *attackingCharacter) {
    double score = 0;

    double healthScore = 1.0 / enemy->getHp();
    score += healthScore * healthScore;

    int speed = 0;
    if (attackingCharacter->getType() == 0) {
        auto *cowboy = dynamic_cast<Cowboy *>(attackingCharacter);
        if (cowboy->hasboolets()) {
            score += 5 * cowboy->getBullets();
        }
    } else if (attackingCharacter->getType() == 1) {
        speed = youngNinjaSpeed;
        if (attackingCharacter->distance(enemy) <= 1) {
            score += 40;
        }
    } else if (attackingCharacter->getType() == 2) {
        speed = trainedNinjaSpeed;
        if (attackingCharacter->distance(enemy) <= 1) {
            score += 40;
        }
    } else if (attackingCharacter->getType() == 3) {
        speed = oldNinjaSpeed;
        if (attackingCharacter->distance(enemy) <= 1) {
            score += 40;
        }
    }

    int nearbyEnemies = 0;
    for (Character *other: enemyTeam->getCharacters()) {
        if (other != enemy && other != nullptr && other->getHp() > 0) {
            double distance = enemy->distance(other);
            if (distance <= speed) {
                nearbyEnemies++;
            }
        }
    }
    score += nearbyEnemies;
    return score;
}

// This function makes the SmartTeam attack the enemy team.
// It does this by iterating over each character in the SmartTeam, and for each character:
// 1. If the character is not alive, it skips the character.
// 2. Otherwise, it calculates a score for each enemy in the enemy team.
// 3. It targets the enemy with the highest score.
// 4. If the character is a Cowboy and has bullets, it makes the Cowboy shoot the targeted enemy.
// 5. If the character is a Ninja, it checks the distance to the targeted enemy.
// 6. If the distance is less than or equal to 1, it makes the Ninja slash the targeted enemy.
// 7. Otherwise, it makes the Ninja move towards the targeted enemy.
// 8. If there are no enemies left alive, it breaks the loop.
void SmartTeam::attack(Team *enemyTeam) {
    //checks before an attack
    swapLeaderIfNeeded();
    if (!enemyTeam) throw std::invalid_argument("a nullptr has been given as a pointer");
    if (enemyTeam->stillAlive() == 0) throw std::runtime_error("cannot attack a dead team");
    if (stillAlive() == 0) return;
    if (this == enemyTeam) throw std::runtime_error("group cannot attck itself");
    for (Character *attackingCharacter: this->getCharacters()) {
        if (attackingCharacter == nullptr || !attackingCharacter->isAlive()) {
            continue;
        }
        std::map<Character *, double> scores;
        for (Character *enemy: enemyTeam->getCharacters()) {
            if (enemy == nullptr || enemy->getHp() <= 0) {
                continue;
            }
            double score = this->enemyScore(enemy, enemyTeam, attackingCharacter);
            scores[enemy] = score;
        }

        Character *bestEnemy = nullptr;
        double bestScore = -1;
        for (const auto &pair: scores) {
            if (pair.second > bestScore) {
                bestEnemy = pair.first;
                bestScore = pair.second;
            }
        }

        if (bestEnemy != nullptr) {
            targetedEnemies.push_back(bestEnemy);
            if (typeid(*attackingCharacter) == typeid(Cowboy)) {
                dynamic_cast<Cowboy *>(attackingCharacter)->shoot(bestEnemy);
            } else if (dynamic_cast<Ninja *>(attackingCharacter) != nullptr) {
                if (attackingCharacter->distance(bestEnemy) <= 1) {
                    dynamic_cast<Ninja *>(attackingCharacter)->slash(bestEnemy);
                } else {
                    dynamic_cast<Ninja *>(attackingCharacter)->move(bestEnemy);
                }
            }
        }
        if (enemyTeam->stillAlive() == 0) {
            break;
        }
    }
    targetedEnemies.clear();
}

SmartTeam::SmartTeam(Character *leader) : Team(leader) {
    targetedEnemies = std::vector<Character *>();
}

void SmartTeam::print() {
    for (size_t i = 0; i < getSize(); ++i) {
        if (getCharacters().at(i) != nullptr) {
            std::cout << getCharacters().at(i)->print() << std::endl;
        }
    }
}
