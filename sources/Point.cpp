#include <iostream>
#include <complex>
#include "Point.hpp"


using namespace ariel;

/**
 * ctr
 * @param xVal coordinate
 * @param yVal coordinate
 */
Point::Point(double xVal, double yVal) : xVal(xVal), yVal(yVal) {}

/**
 * empty ctr default to (0,0)
 */
Point::Point() : xVal(0), yVal(0) {}

/**
 * copy assignment operator
 * @param other
 * @return
 */
Point &Point::operator=(const Point &other) = default;

/**
 *
 * @param other
 */
Point::Point(const Point &other) = default;

/**
 * move constructor
 * @param other
 */
Point::Point(Point &&other) noexcept {
    xVal = other.xVal;
    yVal = other.yVal;
}

/**
 * move assignment
 * @param other
 * @return this
 */
Point &Point::operator=(Point &&other) noexcept {
    if (this != &other) {
        xVal = other.xVal;
        yVal = other.yVal;
    }
    return *this;
}

/**
 * destructor
 */
Point::~Point() = default;

/**
 * check the distance between this point and otherPoint
 * @param otherPoint
 * @return the distance
 */
double Point::distance(const Point &otherPoint) const {
    double xDiff = this->xVal - otherPoint.xVal;
    double yDiff = this->yVal - otherPoint.yVal;
    return std::sqrt(xDiff * xDiff + yDiff * yDiff);
}

/**
 * @return a string representing the point between parenthesis
 */
std::string Point::print() const {
    return "(" + std::to_string(this->xVal) + "," + std::to_string(this->yVal) + ")";
}

/**
 * move to the closest point near the destination based on the distance from the source
 * using euclidean distance
 * @param src point
 * @param dest point
 * @param distance
 * @return the closest point
 */
Point Point::moveTowards(const Point &src, const Point &dest, double distance) {
    if (distance < 0) {
        throw std::invalid_argument("distance can't be below zero");
    }
    double dx = dest.xVal - src.xVal;
    double dy = dest.yVal - src.yVal;
    double magnitude = std::sqrt(dx * dx + dy * dy);
    if (magnitude <= distance) {
        return dest;
    } else {
        double ratio = distance / magnitude;
        return {src.xVal + ratio * dx, src.yVal + ratio * dy};
    }
}

/**
 * @param otherPoint
 * @return check if 2 points have the same values
 */
bool Point::operator==(const Point &otherPoint) const {
    return xVal == otherPoint.xVal &&
           yVal == otherPoint.yVal;
}

/**
 * @param otherPoint
 * @return check if 2 points don't have the same values
 */
bool Point::operator!=(const Point &otherPoint) const {
    return !(otherPoint == *this);
}

/**
 * @return current xVal value
 */
double Point::getX() const {
    return xVal;
}

/**
 * @return current yVal value
 */
double Point::getY() const {
    return yVal;
}








