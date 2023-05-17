#include "YoungNinja.hpp"

using namespace ariel;

/**
 * YoungNinja ctr
 * @param name
 * @param location
 */
YoungNinja::YoungNinja(const std::string &name, const Point &location) : Ninja(
        name, location, youngNinjaHP, youngNinjaSpeed, typeNinja) {}
