#ifndef SYSTEMSEX4A_CHARACTER_HPP
#define SYSTEMSEX4A_CHARACTER_HPP

#include "Point.hpp"
#include <iostream>

namespace ariel {
    enum characterType {
        noType = 0, typeNinja = 1, typeCowboy = 2
    };

    class Character {
    private:
        Point location;
        int hitPoints;
        std::string name;
        enum characterType type;
        bool teamMember;

    public:
        Character(std::string name, Point location, int hitPoints, enum characterType type);

        Character();

        Character &operator=(const Character &other);

        Character(const Character &other);

        Character(Character &&other) noexcept;

        Character &operator=(Character &&other) noexcept;

        virtual ~Character();

        bool isAlive() const;

        double distance(Character *otherChar);

        void hit(int damage);

        virtual std::string print() const;

        const Point &getLocation() const;

        void setLocation(const Point &location);

        int getHp() const;

        const std::string &getName() const;

        characterType getType() const;

        bool isTeamMember() const;

        void setTeamMember();



    };
}


#endif //SYSTEMSEX4A_CHARACTER_HPP
