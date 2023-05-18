#ifndef SYSTEMSEX4A_TEAM_H
#define SYSTEMSEX4A_TEAM_H

#include "Character.hpp"
#include "OldNinja.hpp"
#include "TrainedNinja.hpp"
#include "YoungNinja.hpp"
#include "Cowboy.hpp"
#include "array"

namespace ariel {
    const int maxCharacters = 10;

    class Team {
    private:
        std::array<Character *, maxCharacters> characters{};
        Character *leader;
        size_t size;
    public:
        explicit Team(Character *leader);

        Team(const Team &other);

        Team(Team &&other) noexcept;

        Team &operator=(const Team &other);

        Team &operator=(Team &&other) noexcept;

        void add(Character *character);

        virtual void attack(Team *enemyTeam);

        int stillAlive();

        virtual void print();

        const std::array<Character *, maxCharacters> &getCharacters() const;

        Character *getLeader() const;

        virtual ~Team();

        void setLeader(Character *newLeader);

        void swapLeaderIfNeeded();

        Character *findClosestTarget(Team *team);

        Character *ninjaAttacking(Team *enemyTeam, Character *victim, Character *member);

        Character *cowboyAttacking(Team *enemyTeam, Character *victim, Character *member);

        size_t getSize() const;

    private:
        void deleteArr() const;
    };
}

#endif //SYSTEMSEX4A_TEAM_H
