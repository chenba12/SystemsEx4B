#include <map>
#include "SmartTeam.hpp"

using namespace ariel;

SmartTeam::SmartTeam(Character *leader) : Team(leader) {}



/**
 * Calculates a score for each enemy character.
 * The score is based on the enemy's health, type, and proximity to other enemies and our characters.
 *
 * @param enemy The enemy character whose score we are calculating.
 * @param enemyTeam The team to which the enemy belongs.
 * @return The calculated score for the enemy.
 */
double SmartTeam::calculateEnemyScore(Character *enemy, Team *enemyTeam) {
    double score = 0;

    // Higher score for lower health.
    score += 1.0 / enemy->getHp();

    if (enemy->getType() == typeNinja) {
        score += 1.0;
    }

    if (enemy->getType() == typeCowboy) {
        score += 0.5;
    }

    // Higher score for proximity to other enemies.
    for (Character *other: enemyTeam->getCharacters()) {
        if (other != enemy && other != nullptr && other->getHp() > 0) {
            score += 1.0 / enemy->distance(other);
        }
    }

    // Higher score for proximity to our characters.
    for (Character *ourCharacter: this->getCharacters()) {
        if (ourCharacter != nullptr && ourCharacter->getHp() > 0) {
            score += 1.0 / enemy->distance(ourCharacter);
        }
    }

    return score;
}
//TODO implement
/**
 * does something
 * @param enemyTeam
 */
void SmartTeam::attack(Team *enemyTeam) {
// Loop through each of our characters...
    for (Character *ourCharacter: this->getCharacters()) {
        if (ourCharacter == nullptr) { // Skip any null characters in the team.
            continue;
        }
        if (enemyTeam->stillAlive() <= 0) {
            break;
        }
        // Calculate scores for each enemy.
        std::map<Character *, double> scores;
        for (Character *enemy: enemyTeam->getCharacters()) {
            if (enemy == nullptr || enemy->getHp() <= 0) { // Skip any null or dead enemies.
                continue;
            }

            double score = this->calculateEnemyScore(enemy, enemyTeam);
            scores[enemy] = score;
        }
        // Find the enemy with the highest score.
        Character *bestEnemy = nullptr;
        double bestScore = -1;
        for (const auto &pair: scores) {
            if (pair.second > bestScore) {
                bestScore = pair.second;
                bestEnemy = pair.first;
            }
        }

        // If we found an enemy to attack...
        if (bestEnemy != nullptr) {
            if (ourCharacter->isAlive()) {
                if (ourCharacter->getType() == typeCowboy) {
                    auto *cowboy = dynamic_cast<Cowboy *>(ourCharacter);
                    cowboy->shoot(bestEnemy);
                } else if (ourCharacter->getType() == typeNinja) {
                    auto *ninja = dynamic_cast<Ninja *>(ourCharacter);
                    if (ninja->distance(bestEnemy) <= 1) {
                        ninja->slash(bestEnemy);
                    } else {
                        ninja->move(bestEnemy);
                    }
                }
            }
        }
    }
}

