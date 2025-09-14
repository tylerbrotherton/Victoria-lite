#pragma once

#include <string>
#include <list>
#include <map>
#include <iostream>

#include "country.h"
#include "tile.h"


// Forward declarations - like announcing guests at a party nobody wants to attend
struct Country;
struct Tile;


struct game{
    std::list<Tile> game_map; // The world: now in convenient list format
    std::map<std::string, Country> ledger; // The world's most expensive address book
}

std::string initatesGame();//returns playerTag, because I can't do it another way(I tried)

// Handles the logic for invading another country, but logic is still being ironed out.
void invade(Country& player, std::map<std::string, Country>& current_ledger);

// Save game progress - because nobody wants to relive their mistakes
void saveGameProgress();

//Load game from storage-STILL BETA! It gets wacky regularly. YOU HAVE BEEN WARNED!
// Load game - Resurrection service for your digital empire
bool loadGameProgress();
