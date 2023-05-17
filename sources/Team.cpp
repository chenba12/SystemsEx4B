#include <limits>
#include "Team.hpp"

using namespace ariel;

/**
 * team ctr
 * @param leader the team's leader
 */
Team::Team(Character *leader) {
    std::fill(characters.begin(), characters.end(), nullptr);
    if (leader->isTeamMember()) {
        throw std::runtime_error("Leader already in another team");
    }
    this->leader = leader;
    this->leader->setTeamMember();
    characters.at(0) = this->leader;

}

Team::Team(const Team &other)
        : characters(other.characters),
          leader(new Character(*(other.leader))) {}

Team::Team(Team &&other) noexcept: characters(other.characters),
                                   leader(other.leader) { other.leader = nullptr; }

Team &Team::operator=(const Team &other) {
    if (this != &other) {
        characters = other.characters;
        delete leader;
        leader = new Character(*(other.leader));
    }
    return *this;
}

Team &Team::operator=(Team &&other) noexcept {
    if (this != &other) {
        characters = other.characters;
        leader = other.leader;
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

void Team::deleteArr() const {
    for (auto &character: characters) {
        delete character;
    }
}


/**
 * find an empty place in the team members list and add the character
 * @param character
 */
void Team::add(Character *character) {
    if (character->isTeamMember()) {
        throw std::runtime_error("character already in another team");
    }
    for (size_t i = 0; i < maxCharacters; ++i) {
        if (characters.at(i) == nullptr) {
            characters.at(i) = character;
            character->setTeamMember();
            return;
        }
    }
    throw std::runtime_error("the team is full");
}

/**
 * does something
 * @param enemyTeam
 */
void Team::attack(Team *enemyTeam) {
    if (enemyTeam == nullptr) {
        throw std::invalid_argument("got a nullptr");
    }
    if (enemyTeam->stillAlive() == 0) throw std::runtime_error("Team is dead");
    swapLeaderIfNeeded();
    Character *victim = findClosestTarget(enemyTeam);
    for (size_t i = 0; i < maxCharacters; ++i) {
        if (characters.at(i) != nullptr) {
            victim = cowboyAttacking(enemyTeam, victim, i);
        }
    }
    for (size_t i = 0; i < maxCharacters; ++i) {
        if (characters.at(i) != nullptr) {
            victim = ninjaAttacking(enemyTeam, victim, i);
        }
    }
}

Character *Team::ninjaAttacking(Team *enemyTeam, Character *victim, size_t index) {
    if (characters.at(index)->getType() == typeNinja) {
        auto *ninja = dynamic_cast<Ninja *>(characters.at(index));
        if (victim == nullptr || !victim->isAlive()) victim = findClosestTarget(enemyTeam);
        if (victim != nullptr && ninja->isAlive() && victim->isAlive()) {
            if (ninja->distance(victim) <= 1) ninja->slash(victim);
            else ninja->move(victim);
        }
    }
    return victim;
}

Character *Team::cowboyAttacking(Team *enemyTeam, Character *victim, size_t index) {
    if (characters.at(index)->getType() == typeCowboy) {
        auto *cowboy = dynamic_cast<Cowboy *>(characters.at(index));
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
    for (size_t i = 0; i < maxCharacters; ++i) {
        if (characters.at(i) != nullptr) {
            if (characters.at(i)->isAlive()) {
                counter++;
            }
        }
    }
    return counter;
}

/**
 * print all the characters in the team
 */
void Team::print() {
    for (size_t i = 0; i < maxCharacters; ++i) {
        if (characters.at(i) != nullptr) {
            std::cout << characters.at(i)->print() << std::endl;
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

Character *Team::findClosestTarget(Team *team) {
    double minDistanceFromLeader = std::numeric_limits<double>::max();
    double temp;
    Character *closestCharacter = nullptr;
    for (size_t i = 0; i < maxCharacters; ++i) {
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

void Team::setLeader(Character *newLeader) {
    Team::leader = newLeader;
}

void Team::swapLeaderIfNeeded() {
    if (!this->leader->isAlive()) {
        Character *newLeader = findClosestTarget(this);
        if (newLeader != nullptr)this->setLeader(newLeader);
    }
}







