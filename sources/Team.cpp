#include <limits>
#include "Team.hpp"

using namespace ariel;

/**
 * team ctr
 * @param leader the team's leader
 */
Team::Team(Character *leader) {
    std::fill(characters.begin(), characters.end(), nullptr);
    if (leader == nullptr) {
        throw std::invalid_argument("Got nullptr");
    }
    if (leader->isTeamMember()) {
        throw std::runtime_error("Leader already in another team");
    }
    if (!leader->isAlive()) {
        throw std::runtime_error("Leader is dead");
    }
    this->leader = leader;
    size = 1;
    this->leader->setTeamMember();
    characters.at(0) = this->leader;
}

/**
 * copy ctr
 * @param other
 */
Team::Team(const Team &other)
        : characters(other.characters),
          leader(new Character(*(other.leader))), size(0) {
    for (size_t i = 0; i < maxCharacters; ++i) {
        if (characters.at(i) != nullptr) size++;
    }
}

/**
 * move copy ctr
 * @param other
 */
Team::Team(Team &&other) noexcept: characters(other.characters),
                                   leader(other.leader), size(0) { other.leader = nullptr; }

/**
 *  copy assigment operator
 * @param other
 * @return
 */
Team &Team::operator=(const Team &other) {
    if (this != &other) {
        characters = other.characters;
        delete leader;
        leader = new Character(*(other.leader));
        size = other.size;
    }
    return *this;
}

/**
 * move assignment operator
 * @param other
 * @return
 */
Team &Team::operator=(Team &&other) noexcept {
    if (this != &other) {
        characters = other.characters;
        leader = other.leader;
        size = other.size;
        other.deleteArr();
    }
    return *this;
}

/**
 * destructor
 */
Team::~Team() {
    deleteArr();
}

/**
 * used for destructor
 */
void Team::deleteArr() const {
    for (auto &character: characters) {
        delete character;
    }
}


/**
 * find an empty place in the team members list and add the character
 * @param character
 * @throws runtime_error if the team is full or the character to add is in another team
 */
void Team::add(Character *character) {
    if (character == nullptr) throw std::invalid_argument("got nullptr");
    if (character->isTeamMember())throw std::runtime_error("character already in another team");
    if (!character->isAlive()) throw std::invalid_argument("Dead character");
    if (size == maxCharacters) throw std::runtime_error("the team is full");
    if (characters.at(size) == nullptr) {
        characters.at(size) = character;
        character->setTeamMember();
        size++;
        return;
    }
}

/**
 * use the characters in this team to attack the other team
 * cowboys attack first and then ninjas
 * @param enemyTeam
 * @throws runtime_error if the enemy team is already dead
 * @throws invalid_argument if entered a nullptr
 */
void Team::attack(Team *enemyTeam) {
    if (enemyTeam == nullptr) {
        throw std::invalid_argument("got a nullptr");
    }
    if (enemyTeam->stillAlive() == 0) throw std::runtime_error("Team is dead");
    swapLeaderIfNeeded();
    Character *victim = findClosestTarget(enemyTeam);
    for (size_t i = 0; i < maxCharacters; ++i) {
        Character *member = characters.at(i);
        if (characters.at(i) != nullptr) {
            victim = cowboyAttacking(enemyTeam, victim, member);
        }
    }
    for (size_t i = 0; i < maxCharacters; ++i) {
        Character *member = characters.at(i);
        if (member != nullptr) {
            victim = ninjaAttacking(enemyTeam, victim, member);
        }
    }
}

/**
 * attack the victim if he is alive else find a new victim
 * @param enemyTeam
 * @param victim
 * @param member the character to attack with
 * @return the victim
 */
Character *Team::ninjaAttacking(Team *enemyTeam, Character *victim, Character *member) {
    if (member->getType() == typeNinja) {
        auto *ninja = dynamic_cast<Ninja *>(member);
        if (victim == nullptr || !victim->isAlive()) victim = findClosestTarget(enemyTeam);
        if (victim != nullptr && ninja->isAlive() && victim->isAlive()) {
            if (ninja->distance(victim) <= 1) ninja->slash(victim);
            else ninja->move(victim);
        }
    }
    return victim;
}

/**
 * attack the victim if he is alive else find a new victim
 * @param enemyTeam
 * @param victim
 * @param member the character to attack with
 * @return the victim
 */
Character *Team::cowboyAttacking(Team *enemyTeam, Character *victim, Character *member) {
    if (member->getType() == typeCowboy) {
        auto *cowboy = dynamic_cast<Cowboy *>(member);
        if (victim == nullptr || !victim->isAlive()) victim = findClosestTarget(enemyTeam);
        if (victim != nullptr && cowboy->isAlive() && victim->isAlive()) cowboy->shoot(victim);
    }
    return victim;
}

/**
 * count how many characters are still alive in the team
 * @return the amount of characters alive
 */
int Team::stillAlive() {
    int counter = 0;
    for (size_t i = 0; i < size; ++i) {
        if (characters.at(i) != nullptr) {
            if (characters.at(i)->isAlive()) {
                counter++;
            }
        }
    }
    return counter;
}

/**
 * print all the characters in the team cowboys first and then ninjas
 */
void Team::print() {
    for (size_t i = 0; i < size; ++i) {
        if (characters.at(i) != nullptr) {
            if (characters.at(i)->getType() == typeCowboy) {
                std::cout << characters.at(i)->print() << std::endl;
            }
        }
    }
    for (size_t i = 0; i < size; ++i) {
        if (characters.at(i) != nullptr) {
            if (characters.at(i)->getType() == typeNinja) {
                std::cout << characters.at(i)->print() << std::endl;
            }
        }
    }
}

/**
 * @return an array of characters in the team
 */
const std::array<Character *, maxCharacters> &Team::getCharacters() const {
    return characters;
}

/**
 * @return the team leader
 */
Character *Team::getLeader() const {
    return leader;
}

/**
 * find the closest target to your leader from the other team
 * @param team enemy team
 * @return the character to attack
 */
Character *Team::findClosestTarget(Team *team) {
    double minDistanceFromLeader = std::numeric_limits<double>::max();
    double temp;
    Character *closestCharacter = nullptr;
    for (size_t i = 0; i < team->size; ++i) {
        if (team->characters.at(i) != nullptr) {
            if (team->characters.at(i)->isAlive()) {
                temp = leader->distance(team->characters.at(i));
                if (temp < minDistanceFromLeader) {
                    minDistanceFromLeader = temp;
                    closestCharacter = team->characters.at(i);
                }
            }
        }
    }
    return closestCharacter;
}

/**
 * @param newLeader
 */
void Team::setLeader(Character *newLeader) {
    Team::leader = newLeader;
}

/**
 * swap the leader if it's dead
 */
void Team::swapLeaderIfNeeded() {
    if (!this->leader->isAlive()) {
        Character *newLeader = findClosestTarget(this);
        if (newLeader != nullptr)this->setLeader(newLeader);
    }
}

size_t Team::getSize() const {
    return size;
}





