#include <string>
#include <iostream>
#include "country.h" //this is needed for display number
#include "constants.h"

extern int FACTORY_GDP;
extern int BASE_GDP;

//display a million as 3.2M rather than 3200000 to make it not look like 320000(320K)
// Because nobody has time to count zeros like some kind of medieval accountant
std::string displayNumber(int number);

double tile::getGdpContribution() {
        
        return (BASE_GDP + (factories * FACTORY_GDP)); // Factories add to a tile's GDP - smoke stacks = cash stacks
}

void tile::display() {
        std::string mood;
        if (happiness >= 80) mood = "Happy"; // Living the dream, probably has universal healthcare
        else if (happiness >= 50) mood = "Neutral"; // Meh - the political stance of the 21st century
        else mood = "Unrest"; // Pitchforks and torches time - medieval solutions to modern problems
        std::cout << "    - " << name
                  << " (GDP: $" << displayNumber(getGdpContribution()) // Show me the money!
                  << ", Factories: " << displayNumber(factories) // Environmental destruction counter
                  << ", Barracks: " << displayNumber(barracks) // Professional anger management facilities
                  << ", Happiness: " << mood << "("<<happiness<<")"<<"\n"; // Emotional status report
}
