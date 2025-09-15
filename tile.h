#pragma once // Prevents multiple inclusions
#include <string>


struct Country;

// Represents a single piece of territory
// AKA: "This land is your land, this land is my land... actually, it's probably someone else's land"
struct tile {
    std::string name= "UNKOWN_TILE"; // What to call this patch of dirt
    Country* owner = nullptr; // Like a deed, but with more bloodshed and fewer lawyers
    int factories = 0; // Pollution generators disguised as progress
    int barracks = 0; // Where we train people to professionally disagree with other countries
    int happiness = 70; // Happiness is now per-tile - because misery loves company, but apparently not geography

    tile(std::string n, int inFactories = 0, int inHappiness=70, int inBarracks = 0) : name(n), factories(inFactories), happiness(inHappiness), barracks(inBarracks)  {} // Constructor: some assembly required

    double getGdpContribution();
    void display();

};

