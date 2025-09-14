#pragma once

#include <string>
#include <list>
#include "tile.h"
#include "constants.h"


extern const int GDP_TAX_RATE;
extern const int BARRACKS_COST;
extern const int FACTORY_COST;

// Enum for government types - because democracy is just one flavor of organized chaos
enum class GovernmentType {
    AbsoluteMonarchy, // "I AM the Senate!" - Palpatine's favorite government type
    MilitaryJunta, // Democracy with extra steps and tanks - tanks make everything better
    ConstitutionalMonarchy, // Democracy but with fancy hats and ceremonial swords
    Colony, // Independence is overrated anyway - why think for yourself?
    Democracy, // Two wolves and a sheep voting on dinner - mob rule with better PR
    Communist, // Our bugs, comrade - sharing is caring, especially poverty
    Oligopoly, // Rich people club with political benefits - country club meets country
    Theocracy,
    Anarchist // Government? We don't need no stinking government! - chaos is a ladder
};

// Function to get string representation of GovernmentType 
// Because enums are antisocial and refuse to introduce themselves
std::string to_string(GovernmentType g);

// Represents a nation, which controls a collection of tiles
// Basically a really expensive collection of digital real estate
struct Country {
    std::string countryTag; // Three letters that somehow define your entire national identity
    std::string countryName; // What everyone else calls you (hopefully not behind your back)
    GovernmentType governmentType; // How you've chosen to organize your national dysfunction
    double money; // The root of all evil and/or happiness - depends on how much you have
    std::string atWarWith; // Current nemesis - every good story needs a villain
    bool isPlayer = false; // Are you the chosen one? (Spoiler: probably not)
    std::list<tile*> tiles; // Real estate portfolio - location, location, location!

    Country(std::string tag = "XXX", std::string name = "Unknown", GovernmentType gov = GovernmentType::Colony, int inMoney=12'500)
        : countryTag(tag), countryName(name), governmentType(gov),
          money(inMoney), atWarWith("") {} // Starting with the financial stability of a college student and the military might of a small town
  
    //make sure to ask for GDP and not to save it, because it can change
    // GDP is like a teenager girls mood - constantly fluctuating and impossible to predict
    double getGdp();

    // Change a single tile's happiness
    // Precision happiness adjustment - because sometimes you need to be surgically specific about your emotional manipulation
    void changeTileHappiness(tile* tile, int change) ;
    
    // Blanket happiness changes - because subtlety is overrated
    void changeAllTilesHappiness(int change);

    // Time waits for no nation - except when it's loading
    void tick();

    void processEconomy();

    void checkForRebellion();

    // Construction management - because someone has to build the things
    void build(const std::string& building_type);

    // National status report - everything you never wanted to know about your digital empire
    void displayMyCountry() const;
};
