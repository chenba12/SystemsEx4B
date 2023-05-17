#include "Ninja.hpp"

#include <utility>

using namespace ariel;

/**
 * ninja ctr
 * @param name
 * @param location
 * @param hitPoints
 * @param speed
 * @param type
 */
Ninja::Ninja(std::string name, Point location, int hitPoints, int speed, enum characterType type)
        : Character(std::move(name), std::move(location), hitPoints, type), speed(speed) {}

/**
 * move towards the enemy
 * @param enemy
 * @throws logic_error if the enemy is already dead
 */
void Ninja::move(Character *enemy) {
    if (!enemy->isAlive()) throw std::runtime_error("enemy is dead");
    Point::moveTowards(this->getLocation(), enemy->getLocation(), speed);
}

/**
 * @return the ninja's speed
 */
int Ninja::getSpeed() const {
    return speed;
}

/**
 * slashing an enemy if the distance between the 2 is <1
 * @param enemy
 * @throws logic_error if the target is out of range
 */
void Ninja::slash(Character *enemy) const {
    if (enemy == this) throw std::runtime_error("Can't hit myself :(");
    if (this->getLocation().distance(enemy->getLocation()) < 1) {
        enemy->hit(ninjaDamage);
    } else {
        throw std::logic_error("out of range");
    }
}
