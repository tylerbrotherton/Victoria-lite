#include "tile.h"
#include "game.h"
#include <iostream>

int main() {
    std::cout << "Do you want to 1. start a new game or 2. load a game\n";

    int choice;
    std::cin >> choice;
    std::string playerTag;
    if(choice==1){
        playerTag = initatesGame();//initate game returns player tag btw
    } else if(choice==2){
        std::cout << "This Feature is still experimental, but doesn't overwrite save files\n";
        loadGameProgress(); // this feature is getting coal for christmas
        for (auto& [tag, country] : ledger) {
            if (country.isPlayer) {
                playerTag = tag;
                break;
            }
        }
    } else{
        std::cout << "Invalid input. Crashing;";
	return 1;
    }

    ledger[playerTag].isPlayer = true; // You are the chosen one

    while (true) { // The eternal game loop
        // Check if player country still exists (could be annexed)
        /* Not yet a feature
        if (ledger.find(playerTag) == ledger.end()) {
            std::cout << "Your country has been annexed! Game Over.\n";
            break;
        }
        */
        
        ledger[playerTag].displayMyCountry(); // Your daily dose of nationalism
        std::cout << "--- Main Menu ---\n";
        std::cout << "1. Pass Time (Tick)\n"; // Time travel: the boring edition
        std::cout << "2. View World Ledger\n";
        std::cout << "3. Build Factory\n"; // Environmental destruction: the fun way
        std::cout << "4. Build Barracks\n"; // Job creation for people who enjoy loud noises
        std::cout << "5. Invade Another Country\n"; // Diplomacy: the emergency exit
        std::cout << "6. Save Game\n"; 
        std::cout << "7. Quit Game\n"; // Rage quit: the most honest form of game review

        std::cout << "Enter choice: ";
        std::cout.flush(); // Force output to appear

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n"; // PEBKAC error
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        
        // Clear any remaining input
        std::cin.ignore(10000, '\n');

        if (choice == 1) {
            for (auto& pair : ledger) 
                pair.second.tick(); // Everyone gets older, nobody gets wiser
            std::cout << "A year has passed.\n"; // Time: the ultimate undefeated champion
        } else if (choice == 2) {
            std::cout << "--- Other Countries ---\n";
                for (const auto& [tag, country] : ledger) {
                    country.displayMyCountry(); // Peek at their diary
                }
        } else if (choice == 3) {
            ledger[playerTag].build("factory"); // Industrialization: because someone has to make the widgets
        } else if (choice == 4) {
            ledger[playerTag].build("barracks"); // Military expansion: because peace is just the time between wars
        } else if (choice == 5) {
            invade(ledger[playerTag], ledger); // International relations: the express lane
        } else if (choice == 6) {
	    saveGameProgress();
	    std::cout<<"Saving game\n";
        } else if (choice==7){
            std::cout << "Goodbye!\n"; // Abandoning your digital empire like a responsible adult
            break;
	}
    }
    return 0; // The end of civilization as we know it
}