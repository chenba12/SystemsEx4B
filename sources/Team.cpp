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
    characters[0] = this->leader;

}

Team::Team(const Team &other)
        : characters(other.characters),
          leader(new Character(*(other.leader))) {}

Team::Team(Team &&other) noexcept: characters(other.characters),
                                   leader(other.leader) { other.leader = nullptr; }

//TODO fix
Team &Team::operator=(const Team &other) {
    if (this != &other) {
        characters = other.characters;
        delete leader;
        leader = new Character(*(other.leader));
    }
    return *this;
}

//TODO fix
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
        if (characters[i] == nullptr) {
            characters[i] = character;
            character->setTeamMember();
            break;
        }
    }
    throw std::runtime_error("the team is full");
}

//TODO implement
/**
 * does something
 * @param enemyTeam
 */
void Team::attack(Team *enemyTeam) {
    if (enemyTeam == nullptr) {
        throw std::runtime_error("got a nullptr");
    }
    if (enemyTeam->stillAlive() == 0) throw std::runtime_error("Enemy team is dead");
}

/**
 * count how many characters are still alive in the team
 * @return the amount of characters alive
 */
int Team::stillAlive() {
    int counter = 0;
    for (size_t i = 0; i < maxCharacters; ++i) {
        if (characters[i] != nullptr) {
            if (characters[i]->isAlive()) {
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
        if (characters[i] != nullptr) {
            std::cout << characters[i]->print() << std::endl;
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







