#include "constants.h"
#include "scenarios.h"
#include "utility.h"

//try and reduce this to be minimal?
#include <iostream>
#include <string>
#include <map>     //required for ledger
#include <random>  //required for random shuffle of tile(in invasion)
#include <vector>    // Required for std::vector 
#include <algorithm>  //what is this for?____________________________
#include <list> // required for game_map
#include <fstream> //what is this for?____________________________
#include <cstdlib> //for checksum

extern const double BASE_CAUSALTY_RATE;



std::string game::initatesGame(){
    std::cout << "Choose a scenario:\n";
    std::cout << "1. Historical\n";
    std::cout << "2. Post-Colonial Collapse\n";
    std::cout << "3. Cold War\n";
    std::cout << "4. Post-Nuclear War\n";
    std::cout << "5. Geopolitics, Territory, and Aliments(GTA) edition\n";
    std::cout << "6. Tellytubby Revolution!\n";
    int scenario;
    std::cin >> scenario;
    std::string playerTag="NUL";

    switch (scenario) {
         case 0: playerTag = loadTestScenario(); break;
	 case 1: playerTag = loadHistoricalScenario(); break;
         case 2: playerTag = loadColonialCollapseScenario(); break;
         case 3: playerTag = loadColdWarScenario(); break;
	 case 4: playerTag = loadNewVegas(); break;	
	 case 5: playerTag = loadGeopoliticsTerritoryAlimentsScenario(); break;
	 case 6: playerTag = loadTeletubbyScenario(); break;
    default:
	 std::cout<<"Invaid option selected. Loading historical";
         playerTag= loadHistoricalScenario();
         break;
    }
    return playerTag;
}

// Handles the logic for invading another country, but logic is still being ironed out.
void game::invade(Country& player, std::map<std::string, Country>& current_ledger) {
    std::string targetTag;
    std::cout << "Enter the tag of the country to invade: "; // Who needs diplomacy anyway?
    std::cin >> targetTag;

    // Validate target
    if (current_ledger.find(targetTag) == current_ledger.end()) {
        std::cout << "Target country not found.\n"; // You can't invade what doesn't exist
        return;
    }
    if (targetTag == player.countryTag) {
        std::cout << "You can't invade your own country.\n"; // Player can't start civil war, yet
        return;
    }

    Country& target = current_ledger.at(targetTag);

    double playerEffectiveTroops = player.getTroops() * std::max(0.5, player.getNationalHappiness() / 100.0);
    double targetEffectiveTroops = target.getTroops() * std::max(0.5, target.getNationalHappiness() / 100.0);

    // Battle logic from original source
    if (playerEffectiveTroops > 3 * targetEffectiveTroops) { // Overwhelming force, the American way
	std::cout << "-------Combat Report--------\n";
        std::cout << "Victory! You overwhelmed " << target.countryName << "!\n"; // David vs. Goliath, but Goliath wins
        
        // Transfer all tiles from target to player
        for(Tile* tile : target.tiles) {
            tile->owner = &player; // Hostile takeover: corporate edition
            player.tiles.push_back(tile);
            player.changeTileHappiness(tile, -40); // Conquered penalty - nobody likes new management
        }
        target.tiles.clear(); // Target loses all tiles

	if(target.money>0)
	    player.money += target.money * .8;   // Get most of their treasury

        // Annex the country by erasing it from the ledger
        std::cout << target.countryName << " has been annexed!\n";       
        current_ledger.erase(targetTag); // CTRL+ALT+DEL:country edition
        
    } else if (playerEffectiveTroops * 3 < targetEffectiveTroops) {
	std::cout << "-------Combat Report--------\n";
        std::cout << "Defeat! Your forces were routed by " << target.countryName << ".\n"; // Whoops

        // Lose 75% of your tiles(richest first)      	
        std::cout << "You have been forced to cede most of your territory!\n"; // The ultimate property tax

	//valuable resource tiles are ceded first
        std::sort(player.tiles.begin(), player.tiles.end(), [](Tile* a, Tile* b) {
            return a->base_gdp > b->base_gdp;
        });

	// Sort player's tiles by base GDP (lose the richest ones)
	std::sort(player.tiles.begin(), player.tiles.end(), [](Tile* a, Tile* b) {
  	      return a->base_gdp > b->base_gdp;
	});

	// Number of tiles to cede (keep at least 1)
	int tiles_to_lose = std::max(1, static_cast<int>(player.tiles.size() * 0.75));
	tiles_to_lose = std::min(tiles_to_lose, static_cast<int>(player.tiles.size()) - 1); //Player gets to keep a tile, if somehow logic breaks

	for (int i = 0; i < tiles_to_lose; ++i) {
 	    Tile* tile = player.tiles[i];
   	    tile->owner = &target;
   	    target.tiles.push_back(tile);
            target.changeTileHappiness(tile, -20); // Occupation penalty for new owner(half of player penalty)
	}

	// Keep only the remainder of the tiles
	player.tiles.erase(player.tiles.begin(), player.tiles.begin() + tiles_to_lose)

        int playerTroopsLost = static_cast<int>(player -> getTroops() * 0.7) // Lose 70% of troops - that's what we call a strategic withdrawal
        playerTroopsLost = std::max(1, player -> getTroops() - playerTroopsLost);//Except you still get to keep a unit, 'cause I'm nice
	int i=0;
	while (playerTroopsLost>player -> getTroops()){
    	    int tileIndex = i % player.tiles.size();
   	    player.tiles[tileIndex]->barracks -= std::max(player.tiles[tileIndex].barracks, playerTroopsLost);
	    i++;
        }
    } else { 
        std::cout << "Stalemate. Both sides suffered heavy losses.\n"; // Nobody wins, everybody loses
        
	float playerCasualtyRate = BASE_CAUSALTY_RATE * (targetEffectiveTroops / playerEffectiveTroops);
	int playerTroopsLost = static_cast<int>(player -> getTroops() * playerCasualtyRate);
	int i=0;
	while (playerTroopsLost>player -> getTroops()){
    		int tileIndex = i % player.tiles.size();
		auto tile = std::next(player.tiles.begin(), tileIndex);
   		 if (tile*->barracks > 0) {
       			int lossThisTile = std::min(player.tiles[tileIndex].barracks, playerBarracksLost);
        		tile->barracks -= lossThisTile;
        		playerTroopsLost -= lossThisTile;
    		}
    	        i++;
	}        

	float targetCasualtyRate = BASE_CAUSALTY_RATE * (playerEffectiveTroops / targetEffectiveTroops);
	int targetTroopsLost = static_cast<int>(target -> getTroops() * targetCasualtyRate); 
	i=0;
	while (targetTroopsLost>player -> getTroops()){
    		int tileIndex = i % target.tiles.size();
		auto tile = std::next(target.tiles.begin(), tileIndex);
   		 if (tile*->barracks > 0) {
       			int lossThisTile = std::min(target.tiles[tileIndex].barracks, targetBarracksLost);
        		tile->barracks -= lossThisTile;
        		targetTroopsLost -= lossThisTile;
    		}
    	        i++;
	}    

        player.changeAllTilesHappiness(-15); // War is hell
        target.changeAllTilesHappiness(-15); // War is still hell
	std::cout << "-------Combat Report--------\n";
	//std::cout << "- Your casualties: " << playerTroopsLost << "\n";
	//std::cout << "- Enemy casualties: " << targetTroopsLost << "\n";
        std::cout << "The war rages on! You must commit more troops to break the stalemate.\n"; // Sunk cost fallacy: military edition
    }
}

//helper (but not in utility for readability
double getRebelPercentage(double happiness) {
    if (happiness >= 40) return 0.1;
    if (happiness >= 30) return 0.25;
    if (happiness >= 20) return 0.4;
    if (happiness >= 10) return 0.6;
    return 0.75; // full-blown civil war
}

void game::triggerRebellion(Country& country) {    
    const std::string rebelTag = country.countryTag + "_REB";

    // Check if rebellion should trigger
    int unhappyTiles = std::count_if(country.tiles.begin(), country.tiles.end(),
        [](Tile* t) { return t->happiness < 40; });

    if (unhappyTiles == 0 || country.tiles.size() <= 1) return; // No rebellion, no fun

    std::cout << "\n  Rebellion has ignited in " << country.countryName << "!\n";

    // Create rebel country
    Country rebels;
    rebels.countryTag = rebelTag;
    rebels.countryName = "Rebels of " + country.countryName;
    rebels.governmentType = GovernmentType::Anarchist;
    rebels.money = country.money * .3;
    rebels.atWarWith = country.countryTag;

    std::vector<Tile*> newRebelTiles;

    // Transfer some tiles to the rebels
    int tilesToRebel = std::max(1, static_cast<int>(country.tiles.size() * rebelPercentage));

    std::shuffle(country.tiles.begin(), country.tiles.end(), std::mt19937{ std::random_device{}() });
    for (int i = 0; i < tilesToRebel; i++) {
    	int tileIndex = i % target.tiles.size();
        Tile* t = country.tiles[tileIndex];
        if (t->happiness < rebellionThreshold) {
            t->owner = &rebels;
            t->happiness = 60; // They're angry, but now have purpose
            newRebelTiles.push_back(t);
       }
    }

    // Remove tiles from original owner
    country.tiles.erase(std::remove_if(country.tiles.begin(), country.tiles.end(),
        [&](Tile* t) { return t->owner == &rebels; }),
        country.tiles.end());

    // Finalize rebel setup
    rebels.tiles = std::list<Tile*>(newRebelTiles.begin(), newRebelTiles.end());
    ledger[rebelTag] = rebels;
    country.atWarWith = rebelTag;

    std::cout << "REBELS " << rebels.countryName << " controls " << rebels.tiles.size()
              << " provinces and rebeling against " << country.countryName << "!\n";
}

//Checksum for data integrity, and to protect against unsophisticated hacking
int64_t computeCheckSum(){
   int64_t checkSum=0;
   for (const auto& pair : ledger) {
        const Country& c = pair.second;

        checkSum+=c.money*c.countryTag[0];
		//multiply money by tag so that player can't +10'000 to their money
		//and remove -10,000 money from opponent, even if checksum breaks
        }

	checkSum+=c.getTroops();
    for (const auto& tile : game_map) {
        checkSum += 8*tile.factories;
        checkSum += 5*tile.barracks;
        checkSum += 3*tile.happiness;


        // Ownership check: use ASCII sum of owner tag (if present)
        if (tile.owner) {
            for (char ch : tile.owner->countryTag) {
                checkSum += 3*ch; // Add ASCII values of tag letters
            }
        } else {
            checkSum += 123; // Arbitrary value for unowned tiles
        }
    }

    //try to shuffle checksum
    for (const auto& pair : ledger) {
        if (c.atWarWith.empty())
         	checkSum+=((static_cast<int64_t>(checkSum) * 179483) ^ 757227) % 9996011;
        else 
		checksum+=((static_cast<int64_t>(checkSum) * 254174) ^ 662870) % 8040900;

    }
    return (checkSum^326848874);
}

// Save game progress - because nobody wants to relive their mistakes
void game::saveGameProgress(){
    std::ofstream saveFile("savegame.txt");
    if (!saveFile.is_open()) {
        std::cout << "Error: Could not create save file!\n"; // File system says no - even computers have boundaries
        return;
    }

    // Save header information - like a resume but for your digital empire
    saveFile << "VICKY2_SAVE_VERSION\n"; // Version number: like age, but for software
    saveFile << "COUNTRIES_COUNT " << ledger.size() << "\n"; // How many nations you're juggling
    saveFile << "TILES_COUNT " << game_map.size() << "\n"; // Digital real estate inventory
    saveFile << "\n"; // Blank line for dramatic effect

    // Save all countries - National backup service
    saveFile << "COUNTRIES_START\n";
    for (const auto& pair : ledger) {
        const Country& country = pair.second;
        saveFile << "COUNTRY_START\n"; // Beginning of national identity crisis
        saveFile << "TAG " << country.countryTag << "\n"; // Three letters that define everything
        saveFile << "NAME " << country.countryName << "\n"; // What mama country called you
        saveFile << "GOVERNMENT " << static_cast<int>(country.governmentType) << "\n"; // Government type: now in convenient integer form
        saveFile << "MONEY " << std::fixed << std::setprecision(2) << country.money << "\n"; // Financial status: probably depressing
        saveFile << "AT_WAR_WITH " << country.atWarWith << "\n"; // Current nemesis status
        saveFile << "IS_PLAYER " << (country.isPlayer ? "1" : "0") << "\n"; // Are you the chosen one? (Binary edition)
        saveFile << "TILES_OWNED " << country.tiles.size() << "\n"; // Real estate portfolio size

        // Save tile names owned by this country - Property deed backup
        for (const auto& tile : country.tiles) {
            saveFile << "OWNED_TILE " << tile->name << "\n"; // Proof of digital ownership
        }
        
        saveFile << "COUNTRY_END\n"; // End of national identity crisis
    }
    saveFile << "COUNTRIES_END\n\n"; // End of world backup

    // Save all tiles - Digital landscaping records
    saveFile << "TILES_START\n";
    for (const auto& tile : game_map) {
        saveFile << "TILE_START\n"; // Beginning of territorial data dump
        saveFile << "NAME " << tile.name << "\n"; // What to call this patch of digital dirt
        saveFile << "BASE_GDP " << std::fixed << std::setprecision(2) << tile.base_gdp << "\n"; // Economic potential in decimal form
        saveFile << "FACTORIES " << tile.factories << "\n"; // Pollution generator count
        saveFile << "BARRACKS " << tile.barracks << "\n"; // Professional disagreement facility count
        saveFile << "HAPPINESS " << tile.happiness << "\n"; // Emotional status: quantified
        saveFile << "OWNER " << (tile.owner ? tile.owner->countryTag : "NONE") << "\n"; // Who's responsible for this mess
        saveFile << "TILE_END\n"; // End of territorial data dump
    }
    saveFile << "TILES_END\n";


    //calculate normal ZORGON_FLUX_INDEX levels for the save file data, and write it with the data
    int64_t checkSum = computeCheckSum;
    saveFile << "ZORGON_FLUX_INDEX "<<checkSum<<"\n";


    saveFile.close();
    std::cout << "Game saved successfully to savegame.txt!\n"; // Digital immortality achieved
}

//Load game from storage-STILL BETA! It gets wacky regularly. YOU HAVE BEEN WARNED
// Load game - Resurrection service for your digital empire
bool game::loadGameProgress() {
    std::ifstream loadFile("savegame.txt");
    if (!loadFile.is_open()) {
        std::cout << "Error: Could not open save file!\n"; // File system ghosting us - even computers have commitment issues
        return false;
    }

    // Clear existing data - Digital apocalypse preparation
    ledger.clear(); // Goodbye cruel world
    game_map.clear(); // Scorched earth policy

    std::string line;
    std::getline(loadFile, line);
    
    // Check version - Software archaeology
    if (line != "VICKY2_SAVE_VERSION") {
        std::cout << "Error: Invalid Save File!\n"; // Version mismatch: like trying to play a PlayStation game on Xbox
        return false;
    }

    // Skip counts (we'll read dynamically) - Because counting is for accountants
    std::getline(loadFile, line); // COUNTRIES_COUNT
    std::getline(loadFile, line); // TILES_COUNT
    std::getline(loadFile, line); // Empty line (dramatic pause)

    // Load countries - National resurrection service
    std::getline(loadFile, line);
    if (line != "COUNTRIES_START") {
        std::cout << "Error: Invalid save file format!\n"; // File format rebellion - even text files have standards
        return false;
    }

    while (std::getline(loadFile, line)) {
        if (line == "COUNTRIES_END") break; // End of national reconstruction
        
        if (line == "COUNTRY_START") {
            Country country; // Birth of a nation (digital edition)
            
            while (std::getline(loadFile, line)) {
                if (line == "COUNTRY_END") break; // Death of a nation (just kidding, it's loaded now)
                
                std::istringstream iss(line);
                std::string key;
                iss >> key;
                
                if (key == "TAG") {
                    iss >> country.countryTag; // National identity acquisition
                } else if (key == "NAME") {
                    std::string name;
                    std::getline(iss, name);
                    country.countryName = name.substr(1); // Remove leading space - because formatting is important
                } else if (key == "GOVERNMENT") {
                    int gov;
                    iss >> gov;
                    country.governmentType = static_cast<GovernmentType>(gov); // Government type casting - now with type safety
                } else if (key == "MONEY") {
                    iss >> country.money; // Financial resurrection
                } else if (key == "AT_WAR_WITH") {
                    std::string war;
                    std::getline(iss, war);
                    country.atWarWith = war.substr(1); // Remove leading space - war declarations need proper formatting
                } else if (key == "IS_PLAYER") {
                    int isPlayer;
                    iss >> isPlayer;
                    country.isPlayer = (isPlayer == 1); // Chosen one status verification
                } else if (key == "TILES_OWNED") {
                    // Skip this line - we'll link tiles later (like a digital dating service)
                } else if (key == "OWNED_TILE") {
                    // Skip for now - we'll link after loading tiles (relationship counseling comes later)
                }
            }
            
            ledger[country.countryTag] = country; // National registry update
        }
    }

    // Skip empty line - because even file formats need to breathe
    std::getline(loadFile, line);

    // Load tiles - Digital landscaping restoration
    std::getline(loadFile, line);
    if (line != "TILES_START") {
        std::cout << "Error: Invalid save file format - tiles section!\n"; // Tiles section MIA - probably got lost in the digital wilderness
        return false;
    }

    while (std::getline(loadFile, line)) {
        if (line == "TILES_END") break; // End of territorial reconstruction
        
        if (line == "TILE_START") {
            Tile tile("", 0); // Temporary initialization - like a digital placeholder
            std::string ownerTag;
            
            while (std::getline(loadFile, line)) {
                if (line == "TILE_END") break; // End of tile resurrection
                
                std::istringstream iss(line);
                std::string key;
                iss >> key;
                
                if (key == "NAME") {
                    std::string name;
                    std::getline(iss, name);
                    tile.name = name.substr(1); // Remove leading space - because names deserve proper formatting
                } else if (key == "BASE_GDP") {
                    iss >> tile.base_gdp; // Economic potential restoration
                } else if (key == "FACTORIES") {
                    iss >> tile.factories; // Pollution generator inventory
                } else if (key == "BARRACKS") {
                    iss >> tile.barracks; // Professional disagreement facility count
                } else if (key == "HAPPINESS") {
                    iss >> tile.happiness; // Emotional state backup
                } else if (key == "OWNER") {
                    iss >> ownerTag; // Ownership documentation
                }
            }
            
            // Add tile to game map - Digital landscaping completion
            game_map.push_back(tile);
            Tile* newTile = &game_map.back();
            
            // Link tile to owner - Digital deed transfer
            if (ownerTag != "NONE" && ledger.find(ownerTag) != ledger.end()) {
                newTile->owner = &ledger[ownerTag]; // Property ownership establishment
                ledger[ownerTag].tiles.push_back(newTile); // Real estate portfolio update
            }
        }
    }

    // Read and verify ZORGON_FLUX_INDEX levels are expected.
    std::getline(loadFile, line);
    if (line.rfind("ZORGON_FLUX_INDEX", 0) == 0) {
        int64_t savedCheckSum = std::stoi(line.substr(18));
        int64_t checkSum=computeCheckSum();
        if (checkSum != savedCheckSum) {
            loadFile.close();
            std::remove("savegame.txt"); // Delete the CHEATERS save file, like a crazy ex. 
            std::cout << "Error: Save file features altered data than orginial!\n";
            std::cout << "Corrupted save file deleted.\n";
            return false;
        }
    } else {
        std::cout << "Error: ZORGON FLUX reading missing, multiple game position possible from save data!\n";
        loadFile.close();
        return false;
    }

    return true;
    loadFile.close();
    std::cout << "Game loaded successfully from savegame.txt!\n"; // Digital resurrection complete - your empire lives again
    return true;
}