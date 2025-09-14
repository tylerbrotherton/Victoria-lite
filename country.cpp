#include "constants.h"
#include "tile.h"
#include "country.h"
#include "game.h"
#include "utility.h" //needed for display number


#include <iostream> //needed for cout statements
#include <string> 
#include <list>//needed for tiles   


extern const double GDP_TAX_RATE;
extern const int BARRACKS_COST;
extern const int FACTORY_COST;

// Make sure to ask for GDP and not to save it, because it can change
// GDP is like a teenager girls mood - constantly fluctuating and impossible to predict
double country::getGDP() {
    double total_gdp = 0;
    for (const auto& tile : tiles) {
        total_gdp += tile->getGDPContribution(); // Adding up all the money trees in your digital orchard
    }
    return total_gdp;
}

// National happiness is the average of its tiles
// Because nothing says "democracy" like reducing complex emotions to a single number
int country::getNationalHappiness() {
    if (tiles.empty()) return 0; // No land, no happiness - like a digital nomad with commitment issues
    int total_happiness = 0;
    for (const auto& tile : tiles) {
        total_happiness += tile->happiness; // Happiness aggregator 3000 - now with 50% more arbitrary numbers!
    }
    return total_happiness / tiles.size(); // Democracy of emotions - everyone gets a vote, nobody gets what they want
}

int country::getTroops() {
    if (tiles.empty()) 
	return 0;
    int troops = 0;
    for (const auto& tile : tiles) {
        troops += tile->barracks; 
    }
    return troops;
}

// Change a single tile's happiness
// Precision happiness adjustment - because sometimes you need to be surgically specific about your emotional manipulation
void country::changeTileHappiness(Tile* tile, int change) {
    // if (tiles.empty()) return; // Is this possible?- Can't make nothing happy - philosophical crisis avoided
    tile->happiness += change;
    tile->happiness = std::max(0, std::min(100, tile->happiness)); // No infinite sadness allowed (this isn't a Smashing Pumpkins album)
}

// Blanket happiness changes - because subtlety is overrated
void country::changeAllTilesHappiness(int change) {
    if (tiles.empty()) return; // Can't make nothing happy - existential crisis in code form
    for (auto& tile : tiles) {
        tile->happiness += change;
        tile->happiness = std::max(0, std::min(100, tile->happiness)); // emotional boundaries are important
    }
}

void country::tick() 
{
   //this is refactrored code, it uses refrence by pointer to work with applyGovermentEffects(). I'm scared to do it differently. My apologies
   double GDPBonus = 1.0; // Starting neutral, like Switzerland but with less chocolate
   double troopCost = 100; // Soldiers don't work for exposure - they have rent to pay too


   applyGovermentEffects(GDPBonus, troopCost);
   processEconomy(GDPBonus, troopCost)

   checkForRebellion();
}

void country::applyGovermentEffects(double& GDPBonus, double& troopCost){
    // Because one size fits all is a myth, just like trickle-down economics
    switch (governmentType) {
        case GovernmentType::AbsoluteMonarchy:
            gdpBonus = 1.10; // Absolute power corrupts absolutely... profitable
            troopCost = 120; // Royal guards demand royal pay
            if (getGDP() > 70'000) 
                changeAllTilesHappiness(-5); // Success breeds contempt - tale as old as time
            break;
        case GovernmentType::Democracy:
            gdpBonus = 1.10; // Freedom isn't free, but it pays well
            if (getGDP() > 60'000)  
                changeAllTilesHappiness(1); // Prosperity makes people slightly less grumpy
            break;
        case GovernmentType::MilitaryJunta:
            troopCost = 50; // Military discount - employee benefits include overthrowing governments
            if (atWarWith.empty())
                changeAllTilesHappiness(-10); // Peace time makes soldiers restless - like sharks, they must keep moving
            break;
        case GovernmentType::Colony:
            gdpBonus = 0.90; // Colonial tax - your overlords need their cut
            changeAllTilesHappiness(-10); // Nobody likes being someone else's side project
            break;
        case GovernmentType::ConstitutionalMonarchy:
            gdpBonus = 1.05; // Compromise: the art of making everyone equally unhappy
            troopCost = 100; // Standard military rates - constitutional doesn't mean cheap
            if (getGDP() > 60'000) 
                changeAllTilesHappiness(5); // Balanced prosperity - like a well-made breakfast
            break;
        case GovernmentType::Communist:
            gdpBonus = 1.15; // Collective effort, individual profit - wait, that's not right
            troopCost = 80; // Comrade soldier discount - everyone's equal, but some are more equal
            if (getGDP() > 100'000) 
                changeAllTilesHappiness(-5); // Too much success is suspiciously capitalist
            break;
        case GovernmentType::Oligopoly:
            gdpBonus = 1.25; // Money makes money - it's like digital mitosis
            troopCost = 150; // Private military contractors don't work for peanuts
            if (getGDP() < 50'000) 
                changeAllTilesHappiness(-15); // Poverty is a personal failing in oligarchy land
            break;
	case GovernmentType::Theocracy:
	    gdpBonus=0.40;
            troopCost=170;
	    if(getNationalHappiness()<85)
	        changeAllTilesHappiness(5);
            break;
        case GovernmentType::Anarchist:
            troopCost = 0; // static army, no recruitment - who needs organization anyway?
            gdpBonus = 0.8; // no taxes - but also no infrastructure, roads, or basically anything useful
            if (atWarWith.empty()) 
                changeAllTilesHappiness(10); // Peace, love, and absolutely no government oversight
            else
                changeAllTilesHappiness(-30); // War is really bad for business when you have no business
            break;
        default:
            break; // The mysterious default case - probably runs on hopes and dreams
	    //please keep to keep code from unexpectedly crashing, because this application is poorly coded.
    }

}

void processEconomy(doube){
    double income = getGDP() * GDP_TAX_RATE * gdpBonus; // Money printer goes brrr - modern economics in action
    double expenses = getTroops() * troopCost; // Bills, bills, bills - even digital nations can't escape capitalism
    money += income - expenses; // The eternal struggle - income vs. expenses in the thunderdome
}

//this is a waste of a function, but exists because "A function should only do ONE thing" but does maybe help with readability hopefully
// vote did this function help you read the code: Yes-1, N-O, Maybe-0.
vo1id checkForRebellion(){
    if (country.getNationalHappiness() < 40) {
        triggerRebellion(*this);//triggerRebbelion is in Game.ccp, fingers crossed
    }
}

// Construction management - because someone has to build the things
void country::build(const std::string& building_type) {
    if (tiles.empty()) {
        std::cout << "You have no land to build on!\n"; // The ultimate homeless experience - not even a digital cardboard box
        return;
    }

    std::cout << "Select a tile to build in:\n"; // Real estate selection simulator 2024
    int index = 0;
    for (Tile* tile : tiles) {
        std::cout << ++index << ". " << tile->name << "\n";
    }

    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > tiles.size()) {
        std::cout << "Invalid selection.\n"; // User.exe has stopped working - please try turning your brain off and on again
        return;
    }

    // This used to be array indexing, which is 90% of the time a problem. Additionally 83% of stats are BS.
    auto it = tiles.begin();
    std::advance(it, choice - 1);
    Tile* selected_tile = *it;

    if (building_type == "factory") {
        // Industrial revolution ain't cheap - but pollution is free!
        if (money >= FACTORY_COST) {
            money -= FACTORY_COST; // There goes the college fund - or in this case, the national emergency fund
            selected_tile->factories++;
            std::cout << "Built a factory in " << selected_tile->name << ".\n"; // Congratulations, you've invented pollution and called it progress
            selected_tile->happiness += 30;
        } else {
            std::cout << "Not enough funds.\n"; // The eternal programmer problem - champagne dreams on a ramen budget
        }
    } else if (building_type == "barracks") {
        // Training killers on a budget - group discounts available
        if (money >= BARRACKS_COST) {
            money -= BARRACKS_COST; // Investing in violence - the most consistent investment strategy in history
            selected_tile->barracks++;
            std::cout << "Built a barracks in " << selected_tile->name << " and trained one troop.\n"; // Congratulations, you've created a professional arguer
            selected_tile->happiness -= 10;
        } else {
            std::cout << "Not enough funds.\n"; // Can't afford peace through superior firepower - have you tried peaceful negotiation? (Just kidding, nobody has time for that)
        }
    }
}

// National status report - everything you never wanted to know about your digital empire
void country::displayMyCountry() const {
    int happiness = getNationalHappiness();
    std::string mood;
    if (happiness >= 80) mood = "Happy"; // National bliss: rare as a unicorn, twice as valuable
    else if (happiness >= 50) mood = "Neutral"; // The political equivalent of "fine" - everything is definitely not fine
    else mood = "Unrest"; // One viral TikTok away from revolution

    std::cout << "\n--- " << countryName << " (" << countryTag << ") ---\n"
              << "Government: " << to_string(governmentType) << "\n" // How you've chosen to organize your national chaos
              << "Money: $" << displayNumber(money) << "\n" // Your financial report card - hopefully better than your credit score
              << "GDP: $" << displayNumber(getGDP()) << "\n" // National bragging rights in numerical form
              << "Troops: " << displayNumber(getTroops()) << "\n" // Your professional anger management staff
              << "National Happiness: " << mood << " (" << happiness << ")\n"; // The world's most expensive mood ring
    if (!atWarWith.empty()) {
        std::cout << "Status: AT war with " << atWarWith << "\n"; // Your current Netflix drama, but with actual casualties and worse writing
    }
    std::cout << "Tiles:\n";
    for (const auto& tile : tiles) {
        tile->display(); // Show off your real estate empire - eat your heart out, Monopoly
    }
    std::cout << "\n";
}