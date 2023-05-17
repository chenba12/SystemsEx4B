#include "Character.hpp"

#include <utility>

using namespace ariel;

/**
 * main ctr
 * @param name
 * @param location on the grid
 * @param hitPoints amount of hitPoints
 * @param type
 */
Character::Character(std::string name, Point location, int hitPoints,
                     enum characterType type) : location(std::move(location)),
                                                hitPoints(hitPoints),
                                                name(std::move(name)),
                                                type(type), teamMember(false) {}

/**
 * empty ctr
 */
Character::Character() : location(0, 0), hitPoints(0), type(noType), teamMember(false) {}

Character &Character::operator=(const Character &other) {
    if (this == &other) {
        return *this;
    }

    location = other.location;
    hitPoints = other.hitPoints;
    name = other.name;
    return *this;
}

Character::Character(const Character &other) = default;

Character::Character(Character &&other) noexcept {
    location = other.location;
    hitPoints = other.hitPoints;
    name = std::move(other.name);
    type = other.type;
    teamMember = other.teamMember;
    other.location = Point(0, 0);
    other.hitPoints = 0;
    other.name = "";
    other.type = noType;
    other.teamMember = false;
}

Character &Character::operator=(Character &&other) noexcept {
    if (this != &other) {
        location = other.location;
        hitPoints = other.hitPoints;
        name = std::move(other.name);
        type = other.type;
    }
    return *this;
}

Character::~Character() = default;

/**
 * check if the character is alive
 * @return true if alive false otherwise
 */
bool Character::isAlive() const {
    return hitPoints > 0;
}

/**
 * check the distance from this character to another
 * @param otherChar the target
 * @return the distance between the 2 points
 */
double Character::distance(Character *otherChar) {
    return this->location.distance(otherChar->getLocation());
}

/**
 * remove hitPoints based on the damage param
 * @param damage
 */
void Character::hit(int damage) {
    if (damage < 0) throw std::runtime_error("Damage can't be below zero");
    this->hitPoints -= damage;
}

/**
 * Character info to string
 * @return a string representing the character
 */
std::string Character::print() const {
    std::string typeString;
    if (this->type == typeYoungNinja || this->type == typeOldNinja || this->type == typeTrainedNinja) {
        typeString = 'N';
    } else if (this->type == typeCowboy) {
        typeString = 'C';
    }
    if (this->isAlive()) {
        return "(" + typeString + ") name: (" + getName() + ") hitPoints: " + std::to_string(getHp())
               + " Location: " + getLocation().print();
    }
    return "(" + typeString + ") name: (" + getName()
           + ") Location: " + getLocation().print();
}


/**
 * @return the character current location
 */
const Point &Character::getLocation() const {
    return location;
}

/**
 * @return the character current hitPoints
 */
int Character::getHp() const {
    return hitPoints;
}

/**
 * @return the character name
 */
const std::string &Character::getName() const {
    return name;
}

/**
 * @return the character type
 */
characterType Character::getType() const {
    return type;
}

bool Character::isTeamMember() const {
    return teamMember;
}

void Character::setTeamMember() {
    Character::teamMember = !teamMember;
}







