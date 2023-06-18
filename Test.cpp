#include "doctest.h"

#include "sources/Ninja.hpp"
#include "sources/Point.hpp"
#include "sources/OldNinja.hpp"
#include "sources/YoungNinja.hpp"
#include "sources/TrainedNinja.hpp"
#include "sources/Cowboy.hpp"
#include "sources/Team.hpp"
#include "sources/Team2.hpp"
#include "sources/SmartTeam.hpp"
#include <random>
#include <chrono>

using namespace ariel;

double random_float(double min = -100, double max = 100) {
    std::default_random_engine generator(
            static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> distribution(min, max);

    return distribution(generator);
}

auto create_yninja = [](double x = random_float(), double y = random_float()) {
    return new YoungNinja{"Bob", Point{x, y}};
};

auto create_tninja = [](double x = random_float(), double y = random_float()) {
    return new TrainedNinja{"Bob", Point{x, y}};
};

auto create_oninja = [](double x = random_float(), double y = random_float()) {
    return new OldNinja{"Bob", Point{x, y}};
};

auto create_cowboy = [](double x = random_float(), double y = random_float()) {
    return new Cowboy{"Bob", Point{x, y}};
};

auto random_char(double x = random_float(), double y = random_float()) -> Character * {
    int flag = static_cast<int>(random_float()) % 4;

    if (flag == 0) return create_cowboy(x, y);

    if (flag == 1) return create_yninja(x, y);

    if (flag == 2) return create_tninja(x, y);

    return create_oninja(x, y);
}

auto simulate_battle = [](Team &team, Team &team2) {
    int i = 0;
    while (team.stillAlive() && team2.stillAlive()) {
        if (i % 2 == 0) {
            team.attack(&team2);
        } else {
            team2.attack(&team);
        }
        i++;
    }
};

TEST_CASE("point test") {
    Point p1(0, 0);
    Point p2(3, 4);
    Point p3(-3, -4);
    CHECK_EQ(p1.getX(), 0);
    CHECK_EQ(p1.getY(), 0);
    CHECK_EQ(p1.distance(p2), 5);
    CHECK_EQ(p1.distance(p3), 5);
    CHECK_EQ(p1.print(), "(0.000000,0.000000)");
}

TEST_CASE("character init") {
    Point p(0, 0);
    auto *oldNinja = new OldNinja("oldNinja", p);
    auto *youngNinja = new YoungNinja("youngNinja", {0, 1});
    auto *trainedNinja = new TrainedNinja("trainedNinja", {0, 2});
    auto *cowboy = new Cowboy("cowboy", p);

    CHECK_EQ(oldNinja->getName(), "oldNinja");
    CHECK_EQ(youngNinja->getName(), "youngNinja");
    CHECK_EQ(trainedNinja->getName(), "trainedNinja");
    CHECK_EQ(cowboy->getName(), "cowboy");
    CHECK_EQ(cowboy->getHp(), maxCowboyHP);
    CHECK_EQ(trainedNinja->getHp(), trainedNinjaHP);
    CHECK_EQ(youngNinja->getHp(), youngNinjaHP);
    CHECK_EQ(oldNinja->getHp(), oldNinjaHP);
    CHECK(cowboy->hasboolets());
    CHECK_EQ(trainedNinja->getSpeed(), trainedNinjaSpeed);
    CHECK_EQ(youngNinja->getSpeed(), youngNinjaSpeed);
    CHECK_EQ(oldNinja->getSpeed(), oldNinjaSpeed);
}

TEST_CASE("movement") {
    Point p1(0, 0);
    Point p2(12, 0);
    auto *oldNinja = new OldNinja("oldNinja", p1);
    auto *youngNinja = new YoungNinja("youngNinja", p1);
    auto *trainedNinja = new TrainedNinja("trainedNinja", p1);
    auto *cowboy = new Cowboy("cowboy", p2);
    CHECK_NOTHROW(oldNinja->slash(cowboy));
    oldNinja->move(cowboy);
    oldNinja->slash(cowboy);
    CHECK_EQ(cowboy->getHp(), maxCowboyHP);
    youngNinja->move(cowboy);
    youngNinja->slash(cowboy);
    int hp = maxCowboyHP - ninjaDamage;
    CHECK_LE(cowboy->getHp(), hp);
    trainedNinja->move(cowboy);
    trainedNinja->slash(cowboy);
    hp -= ninjaDamage;
    CHECK_EQ(cowboy->getHp(), hp);
}

TEST_CASE("check distance") {
    Point p1(0, 0);
    Point p2(3, 4);
    auto *oldNinja = new OldNinja("oldNinja", p1);
    auto *cowboy = new Cowboy("cowboy", p2);
    CHECK_EQ(oldNinja->distance(cowboy), 5);
    CHECK_EQ(cowboy->distance(oldNinja), 5);
    oldNinja->move(cowboy);
    CHECK_EQ(oldNinja->distance(cowboy), 0);
    CHECK_EQ(oldNinja->distance(oldNinja), 0);
}

TEST_CASE("moveTowards") {
    Point p1(0, 0);
    Point zero(0, 0);
    Point p2(3, 4);
    Point farPoint(0, 13);

    Point p = Point::moveTowards(p1, p2, 12);
    CHECK_EQ(p, Point{3, 4});
    CHECK_NE(p, Point{3, 5});
    Point p3 = Point::moveTowards(zero, farPoint, 12);
    CHECK_NE(p, Point{0, 13});

}

TEST_CASE("slash and shoot") {
    Point p(0, 0);
    auto *oldNinja = new OldNinja("oldNinja", p);
    auto *youngNinja = new YoungNinja("youngNinja", p);
    auto *cowboy = new Cowboy("cowboy", p);

    while (youngNinja->isAlive()) {
        oldNinja->slash(youngNinja);
    }
    CHECK_NOTHROW(cowboy->reload());
    CHECK_THROWS(oldNinja->slash(youngNinja));
    CHECK_THROWS(cowboy->shoot(youngNinja));
    CHECK_EQ(cowboy->getBullets(), 6);
    CHECK_LE(youngNinja->getHp(), 0);
    CHECK_FALSE(youngNinja->isAlive());
    int hp = oldNinjaHP;
    for (int i = 0; i < 6; ++i) {
        cowboy->shoot(oldNinja);
        CHECK_EQ(oldNinja->getHp(), hp - ((i + 1) * 10));
    }
    CHECK_EQ(cowboy->getBullets(), 0);
    CHECK_FALSE(cowboy->hasboolets());
    CHECK(oldNinja->isAlive());
    CHECK_NOTHROW(cowboy->shoot(oldNinja));
    CHECK(cowboy->hasboolets());
}

TEST_CASE("dead character") {
    Point p(0, 0);
    auto *oldNinja = new OldNinja("oldNinja", p);
    auto *willDie1 = new Cowboy("willDie", p);
    auto *willDie2 = new YoungNinja("willDie2", p);
    while (willDie1->isAlive()) {
        oldNinja->slash(willDie1);
    }
    while (willDie2->isAlive()) {
        oldNinja->slash(willDie2);
    }

    CHECK_THROWS(willDie1->shoot(oldNinja));
    CHECK_THROWS(willDie1->reload());
    CHECK_THROWS(willDie2->slash(oldNinja));
    CHECK_THROWS(willDie2->move(oldNinja));
}

TEST_CASE("damage check") {
    Point p(0, 0);
    auto *dummy = new OldNinja("dummy", p);
    auto *oldNinja = new OldNinja("oldNinja", p);
    auto *cowboy = new Cowboy("cowboy", p);
    int dummyhp = oldNinjaHP;
    cowboy->shoot(dummy);
    dummyhp -= cowboyDamage;
    CHECK_EQ(dummy->getHp(), dummyhp);
    oldNinja->slash(dummy);
    dummyhp -= ninjaDamage;
    CHECK_EQ(dummy->getHp(), dummyhp);
}

TEST_CASE("team setup") {
    Point p1(0, 0);
    auto *youngNinja = new YoungNinja("youngNinja", p1);
    Team team_A(youngNinja);
    CHECK_EQ(team_A.getLeader(), youngNinja);
    CHECK_THROWS(team_A.add(youngNinja));
    for (int i = 0; i < 9; ++i) {
        team_A.add(new YoungNinja("", p1));
    }

    auto young = new YoungNinja("", p1);
    CHECK_THROWS(team_A.add(young)); //team is full
    CHECK_EQ(team_A.stillAlive(), 10);
}


TEST_CASE("team attack") {
    Point p1(0, 0);
    auto *aLeader = new YoungNinja("youngNinja", p1);
    auto *bLeader = new YoungNinja("youngNinja", p1);
    Team teamA(aLeader);
    Team teamB(bLeader);
    CHECK_EQ(teamA.stillAlive(), 1);
    CHECK_THROWS(teamA.add(nullptr));
    CHECK_EQ(teamA.stillAlive(), 1);
    for (int i = 0; i < 9; ++i) {
        if (i >= 5) {
            teamA.add(new Cowboy("", p1));
            teamB.add(new OldNinja("", p1));
        } else {
            teamA.add(new YoungNinja("", p1));
            teamB.add(new OldNinja("", p1));
        }
    }
    while (aLeader->isAlive()) {
        bLeader->slash(aLeader);
    }
    teamA.attack(&teamB);
    CHECK_NE(teamA.getLeader(), aLeader); // check that the leader got replaced

    while (teamA.stillAlive() > 0 && teamB.stillAlive() > 0) {
        teamA.attack(&teamB);
        teamB.attack(&teamA);
    }
    CHECK(((teamA.stillAlive() == 0) || (teamB.stillAlive() == 0)));
    if (teamA.stillAlive() == 0) {
        CHECK_NOTHROW(teamA.attack(&teamB));
    } else if (teamB.stillAlive() == 0) {
        CHECK_NOTHROW(teamB.attack(&teamA));
    }

}

TEST_CASE("team 2") {
    Point p1(0, 0);
    auto *aLeader = new YoungNinja("youngNinja", p1);
    auto *bLeader = new YoungNinja("youngNinja", p1);
    Team2 teamA(aLeader);
    Team2 teamB(bLeader);
    CHECK_EQ(teamA.stillAlive(), 1);
    CHECK_THROWS(teamA.add(nullptr));
    CHECK_EQ(teamA.stillAlive(), 1);
    for (int i = 0; i < 9; ++i) {
        if (i >= 5) {
            teamA.add(new Cowboy("", p1));
            teamB.add(new OldNinja("", p1));
        } else {
            teamA.add(new YoungNinja("", p1));
            teamB.add(new OldNinja("", p1));
        }
    }
    while (aLeader->isAlive()) {
        bLeader->slash(aLeader);
    }
    teamA.attack(&teamB);
    CHECK_NE(teamA.getLeader(), aLeader); // check that the leader got replaced

    while (teamA.stillAlive() > 0 && teamB.stillAlive() > 0) {
        teamA.attack(&teamB);
        teamB.attack(&teamA);
    }
    CHECK(((teamA.stillAlive() == 0) || (teamB.stillAlive() == 0)));
    if (teamA.stillAlive() == 0) {
        CHECK_NOTHROW(teamA.attack(&teamB));
    } else if (teamB.stillAlive() == 0) {
        CHECK_NOTHROW(teamB.attack(&teamA));
    }

}

TEST_CASE("SmartTeam basic functionality") {
    Point p(0, 0);
    auto *leader = new OldNinja("leader", p);
    SmartTeam team(leader);
    CHECK_EQ(team.stillAlive(), 1);
    CHECK_NOTHROW(team.print());
    CHECK_THROWS(team.add(nullptr));
    CHECK_EQ(team.stillAlive(), 1);
    CHECK_EQ(team.getLeader(), leader);
}

TEST_CASE("Smart team") {
    Point p1(0, 0);
    auto *aLeader = new YoungNinja("youngNinja", p1);
    auto *bLeader = new YoungNinja("youngNinja", p1);
    SmartTeam teamA(aLeader);
    SmartTeam teamB(bLeader);
    for (int i = 0; i < 9; ++i) {
        if (i >= 5) {
            teamA.add(new Cowboy("", p1));
            teamB.add(new OldNinja("", p1));
        } else {
            teamA.add(new YoungNinja("", p1));
            teamB.add(new OldNinja("", p1));
        }
    }
    while (aLeader->isAlive()) {
        bLeader->slash(aLeader);
    }
    teamA.attack(&teamB);
    CHECK_NE(teamA.getLeader(), aLeader); // check that the leader got replaced

    while (teamA.stillAlive() > 0 && teamB.stillAlive() > 0) {
        teamA.attack(&teamB);
        teamB.attack(&teamA);
    }
    CHECK(((teamA.stillAlive() == 0) || (teamB.stillAlive() == 0)));
    if (teamA.stillAlive() == 0) {
        CHECK_NOTHROW(teamA.attack(&teamB));
    } else if (teamB.stillAlive() == 0) {
        CHECK_NOTHROW(teamB.attack(&teamA));
    }
}


TEST_CASE("SmartTeam beats Team1 with random characters") {
    SmartTeam smartTeam(random_char());
    Team team1(random_char());

    for (int i = 0; i < 9; ++i) {
        smartTeam.add(random_char());
        team1.add(random_char());
    }

    simulate_battle(smartTeam, team1);
    CHECK_GT(smartTeam.stillAlive(), 0);
    CHECK_EQ(team1.stillAlive(), 0);
}

TEST_CASE("SmartTeam beats Team2 with random characters") {
    SmartTeam smartTeam(random_char());
    Team2 team2(random_char());
    for (int i = 0; i < 9; ++i) {
        smartTeam.add(random_char());
        team2.add(random_char());
    }
    simulate_battle(smartTeam, team2);
    CHECK_GT(smartTeam.stillAlive(), 0);
    CHECK_EQ(team2.stillAlive(), 0);
}


