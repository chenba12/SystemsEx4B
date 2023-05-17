#include "TrainedNinja.hpp"

using namespace ariel;

/**
 * TrainedNinja ctr
 * @param name
 * @param location
 */
TrainedNinja::TrainedNinja(const std::string &name, const Point &location) : Ninja(
        name, location, trainedNinjaHP, trainedNinjaSpeed, typeNinja
) {}
