#include "Cowboy.hpp"

#include <utility>

using namespace ariel;

/**
 * full ctr
 * @param name cowboy name
 * @param location the cowboy's starting location
 */
Cowboy::Cowboy(std::string name, Point location) :
        Character(std::move(name), std::move(location), maxCowboyHP, typeCowboy),
        bullets(maxBullets) {}

/**
 * shoot an enemy character if the cowboy have enough bullets else reload
 * @param enemy
 * @throws logic_error if the enemy is already dead
 */
void Cowboy::shoot(Character *enemy) {
    if (!isAlive())throw std::runtime_error("I'm dead :(");
    if (!enemy->isAlive()) throw std::runtime_error("Enemy is already dead");
    if (enemy == this) throw std::runtime_error("Can't hit myself :(");
    if (hasboolets()) {
        if (enemy->isAlive()) {
            enemy->hit(cowboyDamage);
            bullets--;
        } else throw std::logic_error("the enemy is dead");
    } else {
        this->reload();
    }
}

/**
 * @return if the cowboy have enough bullets
 */
bool Cowboy::hasboolets() const {
    return bullets > 0;
}

/**
 * reload the cowboy's gun
 * @throws logic_error if the cowboy have the max amount of bullets
 */
void Cowboy::reload() {
    if (!isAlive()) throw std::runtime_error("I'm already dead :(");
    bullets = 6;
}

/**
 * @return the current amount of bullets
 */
int Cowboy::getBullets() const {
    return bullets;
}


