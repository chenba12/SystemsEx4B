#include "OldNinja.hpp"

#include <utility>

using namespace ariel;

/**
 * OldNinja ctr
 * @param name
 * @param location
 */
OldNinja::OldNinja(std::string name, Point location) :
        Ninja(std::move(name), std::move(location), oldNinjaHP, oldNinjaSpeed, typeNinja) {}

