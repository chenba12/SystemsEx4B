#ifndef SYSTEMSEX4A_NINJA_HPP
#define SYSTEMSEX4A_NINJA_HPP

#include "Character.hpp"

namespace ariel {
    const int youngNinjaSpeed = 14;
    const int trainedNinjaSpeed = 12;
    const int oldNinjaSpeed = 8;
    const int youngNinjaHP = 100;
    const int trainedNinjaHP = 120;
    const int oldNinjaHP = 150;
    const int ninjaDamage = 40;

    class Ninja : public Character {
    private:
        const int speed;
    protected:
        Ninja(std::string name, Point location, int hitPoints,
              int speed, enum characterType type);

    public:
        void move(Character *enemy);

        void slash(Character *enemy);

        int getSpeed() const;

    };
}

#endif
