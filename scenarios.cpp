#include "game.h"
#include <iostream>
#include <maps>
#include <string> // Ah yes, the classic typo that haunts every C++ programmer's dreams

bool NCRcitizenDeclaration();

// Helper/Lambda function: because regular functions are too mainstream
auto addTile = [](std::string name, std::string tag, double factories = 0, int happiness = 70, int barracks = 0) {
    game_map.push_front(Tile(name, factories)); // Adding tiles like we're playing Minecraft, but with more existential dread
    Tile* tile = &game_map.front(); // Getting a pointer faster than my ex got my Netflix password
    tile->owner = &ledger[tag]; // Assigning ownership like a feudal lord distributing peasants
    tile->happiness = happiness; // Default happiness: 70. Still better than my job satisfaction
    tile->barracks = barracks; // Military buildings: because peace was never an option
    ledger[tag].tiles.push_back(tile); // Adding to the collection like Pokemon cards, but with war crimes
};

std::string inputPrint(std::string printMessage){
    std::string input;
    std::cout<<printMessage;
    std::getline(std::cin, input);
    return input;
}

// The democratic process: now with 100% more user input validation!
std::string selectPlayer(){
    if (ledger.empty()) {
        std::cout << "No countries available!\n"; // The ultimate existential crisis: no nations to rule
        return ""; // Returning emptiness, much like my soul after debugging this
    }

    std::cout << "\n--- Select Your Country ---\n";
    for (const auto& [tag, country] : ledger) { // C++17 structured bindings: fancy destructuring for fancy people
        std::cout << "Tag: " << tag << "\n";
        country.displayMyCountry(); // "Look at my country! LOOK AT IT!" - every player, probably
    }

    std::string selectedTag;
    std::cout << "Enter the tag of the country you'd like to play (e.g., USA, GBR, IND): ";
    std::cin >> selectedTag; // User input: the most dangerous part of any program

    // Convert to uppercase for forgiving input (optional)
    std::transform(selectedTag.begin(), selectedTag.end(), selectedTag.begin(), ::toupper); // SCREAMING YOUR COUNTRY NAME INTO EXISTENCE

    if (ledger.find(selectedTag) == ledger.end()) {
        std::cout << "Invalid tag. Defaulting to "+ledger.begin().tag+"\n"; // "You chose... poorly" - Indiana Jones, probably
        selectedTag = ledger.begin()->first; // Forced democracy: you get the first country whether you like it or not
    }

    // Clear all isPlayer flags just in case
    for (auto& [_, country] : ledger) // The underscore: universal symbol for "I don't care about this variable"
        country.isPlayer = false; // UN-CHOOSING everyone like a brutal game of musical chairs

    ledger[selectedTag].isPlayer = true; // THE CHOSEN ONE has been selected
    return selectedTag; // Returning your new digital identity
}

//sets up board, countries, tile, selects player tag, and returns player tag(which is a string like "USA")
// Game initialization: like creating the universe, but with less cosmic radiation
std::string loadTestScenario(){
    ledger.clear(); // Wiping the slate clean like my browser history
    game_map.clear(); // Tabula rasa, but for imaginary countries

    ledger["USA"] = Country("USA", "United States", GovernmentType::Democracy, 100'000'000); // Freedom: now with 30% more marketing and 50% more existential dread
    ledger["BEL"] = Country("BEL", "Belgium", GovernmentType::Oligopoly, -1'000'000); // Waffle supremacy through economic manipulation - syrup sold separately
    ledger["CAN"] = Country("CAN", "Canada", GovernmentType::Colony, -1'000'000); // America's polite upstairs neighbor who definitely doesn't have oil

    addTileToCountry("New York","USA",5, 100); // The city that never sleeps because it's too expensive to afford a bed
    addTileToCountry("Pennsylvania","USA",5 , 100, 10); // Where the Liberty Bell cracked under pressure - metaphor for American democracy

    addTileToCountry("Wallonia","BEL",3, 100); // That typo in BEL: is going to haunt us like that embarrassing thing you did in high school
    addTileToCountry("Quebec","CAN", 1, 100); // French Canada: the best Canada, don't @ me
    addTileToCountry("Ontario","CAN" 1, 100); // The other Canada - still polite, slightly less French

    std::cout << "Test scenario loaded.\n"; // Announcing completion like a proud parent at a school play
    return selectPlayer;
}

//starting year is roughly June of 1836(cornonation of queen victoria), of real world
std::string loadHistoricalScenario() {
    ledger.clear();
    game_map.clear();
    
    // The Great Powers of 1836
    ledger["GBR"] = Country("GBR", "United Kingdom", GovernmentType::ConstitutionalMonarchy, 40'000); // Workshop of the world, ruler of waves
    ledger["FRA"] = Country("FRA", "France", GovernmentType::ConstitutionalMonarchy, 28'000); // Post-Napoleon constitutional experiment with revolutionary undertones
    ledger["AUS"] = Country("AUS", "Austrian Empire", GovernmentType::AbsoluteMonarchy, 20'000); // Multi-ethnic powder keg held together by Habsburg stubbornness
    ledger["RUS"] = Country("RUS", "Russian Empire", GovernmentType::AbsoluteMonarchy, 18'000); // Autocratic colossus with more serfs than sense
    ledger["PRU"] = Country("PRU", "Prussia", GovernmentType::AbsoluteMonarchy, 22'000); // Military state cosplaying as a country, with extra pickelhaubes
    
    // Secondary Powers
    ledger["USA"] = Country("USA", "America", GovernmentType::Democracy, 4'000);    
    ledger["OTT"] = Country("OTT", "Ottoman Empire", GovernmentType::AbsoluteMonarchy, 7'000); // The sick man of Europe, but still kicking
    ledger["SAR"] = Country("SAR", "Sardinia-Piedmont", GovernmentType::ConstitutionalMonarchy, 6'000); // Italian unification's ambitious little engine
    ledger["SPA"] = Country("SPA", "Spain", GovernmentType::AbsoluteMonarchy, 3'000); // Former colonial giant now nursing imperial hangovers
    
    // German Confederation Members
    ledger["BAV"] = Country("BAV", "Bavaria", GovernmentType::ConstitutionalMonarchy, 8'000); // Beer, catholicism, and mild Austrian sympathies
    ledger["WUR"] = Country("WUR", "Württemberg", GovernmentType::ConstitutionalMonarchy, 7'000); // Small but proud, like a dachshund with delusions of grandeur
    
    // United Kingdom - Industrial powerhouse
    addTile("London", "GBR", 8, 75, 2); // Where the sun never sets on factory smoke
    addTile("Manchester", "GBR", 6, 65, 1); // Cotton mills and child labor, the Victorian dream
    addTile("Birmingham", "GBR", 5, 70, 1); // Making everything from buttons to bayonets
    addTile("Liverpool", "GBR", 4, 68, 1); // Slave trade profits now funding legitimate commerce
    addTile("Edinburgh", "GBR", 2, 72, 1); // Enlightenment meets industrialization, with bagpipes
    addTile("Dublin", "GBR", 1, 45, 2); // Resentful Irish under British rule, potato dependency high
    
    // France - Revolutionary legacy meets bourgeois monarchy
    addTile("Paris", "FRA", 6, 65, 2); // Where revolutions go to practice their choreography
    addTile("Lyon", "FRA", 4, 70, 1); // Silk production and worker organization
    addTile("Marseille", "FRA", 3, 68, 1); // Mediterranean trade and revolutionary songs
    addTile("Bordeaux", "FRA", 2, 75, 1); // Wine, colonial trade, and moderate politics
    addTile("Lille", "FRA", 3, 60, 1); // Industrial north, coal dust, and socialism brewing
    addTile("Strasbourg", "FRA", 1, 65, 1); // Franco-German border tensions in architectural form
    
    // Austrian Empire - Multi-ethnic administrative nightmare
    addTile("Vienna", "AUS", 4, 70, 2); // Imperial capital where waltzes mask political paralysis
    addTile("Prague", "AUS", 3, 55, 1); // Czech nationalism simmering under German administration
    addTile("Budapest", "AUS", 2, 50, 1); // Hungarian nobles plotting independence over goulash
    addTile("Milan", "AUS", 3, 45, 2); // Italian resentment with Austrian efficiency
    addTile("Venice", "AUS", 2, 40, 1); // Former maritime republic now landlocked in spirit
    addTile("Krakow", "AUS", 1, 55, 1); // Free city caught between empires, like a political hot potato
    
    // Russian Empire - Autocracy, orthodoxy, and really big maps
    addTile("St. Petersburg", "RUS", 3, 60, 3); // Imperial capital built on bones and Baltic ambition
    addTile("Moscow", "RUS", 2, 65, 2); // Ancient capital where Orthodox tradition meets autocratic innovation
    addTile("Kiev", "RUS", 1, 55, 1); // Ukrainian heartland under Russian rule
    addTile("Warsaw", "RUS", 2, 35, 2); // Polish independence dreams crushed by Cossack reality
    addTile("Riga", "RUS", 1, 50, 1); // Baltic Germans administering Latvian peasants for Russian tsars
    addTile("Finland", "RUS", 1, 70, 1); // Autonomous grand duchy with Swedish nostalgia
    
    // Prussia - The army with a state attached
    addTile("Berlin", "PRU", 4, 70, 3); // Prussian efficiency meets German philosophy
    addTile("Königsberg", "PRU", 2, 65, 2); // East Prussian stronghold of Junker militarism
    addTile("Breslau", "PRU", 2, 60, 1); // Silesian industrial potential in German hands
    addTile("Cologne", "PRU", 3, 75, 1); // Rhineland prosperity with French revolutionary memories
    addTile("Danzig", "PRU", 2, 55, 1); // German city ruling Polish hinterland
    
    //America - Now with 30% more freedom
    addTile("New York", "USA", 4, 70, 1); 
    addTile("Illinois", "USA", 1, 90, 01; 
    addTile("Virginia", "USA", 1, 30, 4); 

    // Ottoman Empire - Declining but defiant
    addTile("Constantinople", "OTT", 3, 60, 2); // Where Europe meets Asia and empires go to argue
    addTile("Smyrna", "OTT", 2, 65, 1); // Greek merchants under Turkish rule
    addTile("Damascus", "OTT", 1, 55, 1); // Arab provinces dreaming of independence
    addTile("Baghdad", "OTT", 1, 50, 1); // Mesopotamian administration by Ottoman bureaucrats
    addTile("Belgrade", "OTT", 1, 40, 2); // Serbian nationalism under Turkish military oversight
    
    // Sardinia-Piedmont - Italian unification's scrappy underdog
    addTile("Turin", "SAR", 2, 70, 1); // Savoy ambition meets Italian nationalism
    addTile("Genoa", "SAR", 3, 75, 1); // Maritime commerce funding territorial dreams
    addTile("Sardinia", "SAR", 0, 60, 1); // Island kingdom providing royal legitimacy
    
    // Spain - Imperial decline with Bourbon drama
    addTile("Madrid", "SPA", 2, 60, 2); // Royal court where Carlist plots simmer
    addTile("Barcelona", "SPA", 3, 65, 1); // Catalan industry and separatist stirrings
    addTile("Seville", "SPA", 1, 70, 1); // Colonial trade memories and agricultural tradition
    addTile("Cadiz", "SPA", 1, 65, 1); // Atlantic gateway to fading American empire
    
    // Bavaria - Catholic resistance to Prussian Protestant hegemony
    addTile("Munich", "BAV", 2, 75, 1); // Beer halls where Catholic conservatism meets German culture
    addTile("Nuremberg", "BAV", 2, 70, 1); // Imperial city history with modern industrial potential
    
    // Württemberg - Constitutional monarchy in miniature
    addTile("Stuttgart", "WUR", 1, 80, 1); // Small state efficiency with German cultural ambition
    
    // Historical Tensions Setup
    ledger["RUS"].atWarWith = "OTT"; // Eastern Question heating up like Turkish coffee
    ledger["OTT"].atWarWith = "RUS"; // Crimean tensions and Balkan powder keg politics
    
    ledger["AUS"].atWarWith = "SAR"; // Italian nationalism vs Austrian administrative stubbornness
    ledger["SAR"].atWarWith = "AUS"; // Risorgimento dreams backed by Sardinian military modernization
    
    // Potential future conflicts brewing (not active wars)
    // German Confederation tensions between Austria and Prussia
    // Polish nationalism in Russian partition
    // Hungarian independence movement in Austrian Empire
    // Irish nationalism in British Isles
    // Belgian independence secured but Dutch resentment lingers
    
    std::cout << "Victoria II 1836 Grand Campaign scenario loaded.\n";
    std::cout << "The Concert of Europe maintains uneasy peace while nationalism and liberalism spread.\n";
    std::cout << "Industrial revolution accelerates social change and great power competition.\n";
    
    return selectPlayer();
}

// UK collapses before Victoria is cornated, French revolution actually succeds, Conferderates win early civil war(but collapse).
// Alternate history: because regular history wasn't depressing enough
std::string setupColonialCollapse() {
    ledger.clear(); // Historical revisionism starts with a clean slate
    game_map.clear(); // Ctrl+Alt+Del for empires

    // Major powers + rebels (aka: the gang's all here for maximum chaos)
    ledger["USA"] = Country("USA", "United States", GovernmentType::Democracy, 10'000); // Still trying to figure out this whole "freedom" thing
    ledger["CSA"] = Country("CSA", "Confederate States", GovernmentType::Anarchist, 2'000); // States' rights to be chaotic neutral
    ledger["GBR"] = Country("GBR", "United Kingdom", GovernmentType::Oligopoly, 4'000); // Tea-fueled corporate overlords
    ledger["IND"] = Country("IND", "Free India", GovernmentType::Communist, 3'500); // Finally free to redistribute the curry
    ledger["FRC"] = Country("FRC", "French Commune", GovernmentType::Communist, 5'000); // Guillotines for everyone! Equality through decapitation!
    ledger["CAT"] = Country("CAT", "Catalonia", GovernmentType::Anarchist, 1'500); // Because Spain wasn't confusing enough
    ledger["AFR"] = Country("AFR", "African Coalition", GovernmentType::MilitaryJunta, 4'500); // United by the shared experience of colonial nonsense

    // USA (still manifesting destiny, just with more anxiety)
    addTile("New York", "USA", 5, 1); // The Big Apple, now with 100% more urban decay
    addTile("Pennsylvania", "USA", 3); // Liberty Bell still cracked, democracy still questionable
    addTile("Ohio", "USA", 2); // Ohio: the most aggressively mid state in the union

    // CSA (the rebellion that peaked too early)
    addTile("Georgia", "CSA", 2, 40, 1); // Happiness: 40. That's what happens when your economic model is morally bankrupt
    addTile("Tennessee", "CSA", 1, 45, 1); // Slightly happier, still problematic
    ledger["USA"].atWarWith = "CSA"; // Brother against brother, but make it a strategy game
    ledger["CSA"].atWarWith = "USA"; // The feeling is mutual, unfortunately

    // UK (Empire where the sun finally set)
    addTile("London", "GBR", 6, 45, 1); // Still convinced they're relevant
    addTile("Manchester", "GBR", 3, 40); // Industrial revolution's greatest hits

    // India (finally free to spice things up)
    addTile("Delhi", "IND", 3, 65); // Capital of "we told you so"
    addTile("Mumbai", "IND", 2, 60); // Bollywood but make it communist

    // French Commune (revolution 2: electric boogaloo)
    addTile("Paris", "FRC", 4, 45, 2); // City of love, now with 100% more class consciousness
    addTile("Lyon", "FRC", 2, 50, 1); // Silk production meets seizing the means

    // Catalonia (independence through pure stubbornness)
    addTile("Barcelona", "CAT", 2, 40, 1); // Gaudí architecture powered by anarchist energy

    // Africa (the continent finally gets its act together)
    addTile("Lagos", "AFR", 1, 50, 1); // Nigeria leading the charge
    addTile("Nairobi", "AFR", 0, 50, 1); // Has barracks (because someone has to keep the peace)

    std::cout << "Colonial Collapse scenario loaded.\n"; // History teachers everywhere weep
    return selectPlayer;
}

//roughly based on the real world 1953/1954(death of stalin, polio vaccine, brown v. board, rock'n'roll emergence)
// The Cold War: now with 100% more paranoia and 50% less actual 
fighting
std::string loadColdWarScenario() {
    ledger.clear(); // Clearing the board for ideological warfare
    game_map.clear(); // Tabula rasa for the nuclear age

    // The gang's all here for mutually assured destruction
    ledger["USA"] = Country("USA", "United States", GovernmentType::Democracy, 40'000); // Freedom fries and existential anxiety
    ledger["SOV"] = Country("SOV", "Soviet Union", GovernmentType::Communist, 18'000); // In Soviet Russia, country plays you!
    ledger["GER"] = Country("WGR", "West Germany", GovernmentType::Democracy, 10'000); // The good Germany (at least this week)
    ledger["GDR"] = Country("EGR", "East Germany", GovernmentType::Communist, 4'000); // The other Germany (now with more surveillance!)
    ledger["CHN"] = Country("CHN", "People's China", GovernmentType::Communist, 5'000); // Making everything, including ideology
    ledger["JAP"] = Country("JAP", "Japan", GovernmentType::Oligopoly, 8'000); // Corporate samurai in business suits
    ledger["KOR"] = Country("KOR", "South Korea", GovernmentType::Democracy, 2'000); // K-pop hasn't been invented yet, but the potential is there
    ledger["PRK"] = Country("PRK", "North Korea", GovernmentType::MilitaryJunta, 3'500); // Best Korea, according to Best Korea
    ledger["VNM"] = Country("VNM", "Vietnam", GovernmentType::Anarchist, 2'000); // Jungle warfare meets bureaucratic nightmare
    ledger["UNK"] = Country("UAS", "Unaligned States", GovernmentType::Colony, 100, 3'000); // Switzerland energy but make it global

    // USA (spreading democracy one proxy war at a time)
    addTile("California", "USA", 5, 50, 1); // Where the magic of political theater happens
    addTile("New York", "USA", 5, 65 ,1); // Wall Street: capitalism's final boss level

    // USSR (communism with Russian characteristics)
    addTile("Moscow", "SOV", 5, 50, 2); // Red Square, but make it a game tile
    addTile("Kiev", "SOV", 3, 40, 1); // Little did they know what was coming in the 2020s...

    // West & East Germany (the ultimate awkward roommate situation)
    addTile("Bonn", "WGR", 3); // West Germany's backup capital (Berlin was complicated)
    addTile("Leipzig", "EGR", 2, 45, 1); 

    // China (the middle kingdom goes full communist)
    addTile("Beijing", "CHN", 4, 50, 1); // Forbidden City, now with more people's liberation
    addTile("Shanghai", "CHN", 3, 45, 2); // Future economic powerhouse, current revolutionary stronghold

    // Japan (from samurai to salarymen in record time)
    addTile("Tokyo", "JAP", 5); // Godzilla's favorite stomping ground

    // Korea (the peninsula that couldn't decide)
    addTile("Seoul", "KOR", 2); // Future home of K-pop and existential corporate dread
    addTile("Pyongyang", "PRK", 1, 35, 1); // Juche philosophy meets military aesthetics

    // Vietnam (the jungle that ate empires for breakfast)
    addTile("Saigon", "VNM", 1, 30, 2); // About to become Ho Chi Minh City in the most expensive way possible

    // Unaligned (the smart kids who stayed out of the playground fight)
    addTile("Kinshasa", "UAS", 1, 50); // Congo: rich in resources, complicated in everything else
    addTile("Jakarta", "UAS", 1, 60); // Indonesia: too busy with archipelago management to pick sides

    std::cout << "Cold War scenario loaded.\n"; // Duck and cover, it's ideology time!
    return selectPlayer;
}

// Post-apocalyptic political simulation: because nothing says "fun" like nuclear wasteland governance
std::string loadNewVegas() {
    ledger.clear(); // Clearing the board for post-nuclear politics
    game_map.clear(); // War... war never changes, but maps do

    // The wasteland's greatest political minds (and Caesar cosplayers)
    ledger["NCR"] = Country("NCR", "New California Republic", GovernmentType::Democracy, 20'000); // Democracy: now with more bureaucratic incompetence!
    ledger["LGE"] = Country("LGE", "Caesar's Legion", GovernmentType::AbsoluteMonarchy, 10'000); // LARP-ing as Romans because subtlety is dead
    ledger["BOS"] = Country("BOS", "Brotherhood of Steel", GovernmentType::Oligopoly, 6'000); // Tech bros with power armor and superiority complexes
    ledger["VEG"] = Country("NWS", "New Vegas Strip(Mr. House loyalists)", GovernmentType::Anarchist, 14'000); // Capitalism without regulations: the natural endpoint
    ledger["KHN"] = Country("KHN", "Great Khans", GovernmentType::Anarchist, 2'500); // Mad Max but with more substance abuse
    ledger["BLT"] = Country("BLT", "Boomers at Nellis", GovernmentType::Theocracy, 4'000); // Explosive fundamentalism (literally)
    ledger["YES"] = Country("YES", "Yes Man Uprising", GovernmentType::Anarchist, 8'000); // AI revolution led by the most cheerful robot ever

    addTile("Shady Sands", "NCR", 4, 60, 1);         // Capital of gridlock and regulatory capture
    addTile("Boulder City", "NCR", 3, 55, 1);        // Where democracy goes to die slowly in committee
    addTile("Camp McCarran", "NCR", 2, 60, 1);
    addTile("Hoover Dam", "NCR", 5, 80, 2);

    addTile("Flagstaff", "LGE", 2, 40);           // Unhappy whipped slaves (shocking, I know)
    addTile("The Fort", "LGE", 1, 35, 4);         // Barracks = cosplay arena for adult Romans

    addTile("Hidden Valley", "BOS", 2, 60);       // Underground bunker full of smug engineers
    addTile("Old Bunker", "BOS", 1, 55);          // More bunkers, because commitment to the aesthetic

    addTile("The Strip", "VEG", 3, 80);           // Casinos and broken dreams: the American way
    addTile("Freeside", "VEG", 2, 40, 1);            // Where mob rule meets nonprofit chaos

    addTile("Red Rock Canyon", "KHN", 1, 100);     // They have drugs and not much else (living the dream)

    addTile("Nellis Air Force Base", "BLT", 2, 50, 1);  // Boom boom baby - the theological argument for explosives

    addTile("Lucky 38 Mainframe", "YES", 1, 90, 1);   // AI democracy utopia (what could go wrong?)

    // War setup (because peace was never an option in the wasteland)
    ledger["NCR"].atWarWith = "LGE"; // Bureaucracy vs. Cosplay: the ultimate showdown
    ledger["LGE"].atWarWith = "NCR"; // Revenge for being mocked on the wasteland's only podcast

    std::cout << "Fallout: Burnout Vegas loaded. May the least dysfunctional win.\n"; // War... war never changes, but the forms you fill out do
    std::string player = selectPlayer(); // Choose your post-apocalyptic governmental dysfunction

    if(player=="NCR")
	NCRcitizenDeclaration();
    return player;
}

std::string loadGeopoliticsTerritoryAlimentsScenario() {
    ledger.clear();
    game_map.clear();

    //  Grand Capital Theft
    ledger["LSC"] = Country("LSC", "Los Santos Pact", GovernmentType::Oligopoly); // Corporations and criminals now work together — like always, but louder
    ledger["BLC"] = Country("BLC", "Blaine County Militia", GovernmentType::Anarchist); // Off-grid freedom fighters with poor hygiene and too many guns
    ledger["RDR"] = Country("RDR", "New West Federation", GovernmentType::AbsoluteMonarchy, 500'000); // Cowboy monarchists trying to bring back the wild west
    ledger["NTY"] = Country("NTY", "North Yankton", GovernmentType::Colony, 2'000'000); // Cold, forgotten, suspiciously full of money laundering
    ledger["MAX"] = Country("MAX", "Painkiller Republic", GovernmentType::MilitaryJunta, 5'000); // Eternal civil war driven by noir narration and painkillers
    ledger["BLY"] = Country("BLY", "Bullytown Education Board", GovernmentType::MilitaryJunta; // Schoolyard fundamentalists with a theology based on detention slips
    ledger["RSC"] = Country("RSC", "Rockstar Central Authority", GovernmentType::Oligopoly, 40'000); // Votes bought with DLC; propaganda disguised as patch notes

    // Los Santos Conglomerate (corporate-criminal hybrid)
    addTile("Vinewood", "LSC", 5, 70, 1); // Fake Hollywood where egos and cocaine flow freely
    addTile("Del Perro", "LSC", 3, 65, 1); // Gentrified, monetized, and probably on Instagram

    // Blaine County Militia (chaotic rural insurgents)
    addTile("Sandy Shores", "BLC", 2, 40, 1); // Meth labs and freedom flags
    addTile("Grapeseed", "BLC", 1, 45); // Where nobody knows how taxes work, or what they are

    //  New West Federation (Red Dead forever)
    addTile("New Austin", "RDR", 2, 60); // Horse-only transit system
    addTile("Blackwater", "RDR", 3, 55, 3); // The only bank without an online portal

    addTile("Tahiti", "RDR", 0, 95); //TAHBITI ARTFUR

    // North Yankton (frozen crime capital)
    addTile("Ludendorff", "NTY", 1, 45, 0); // Cold, isolated, and suspiciously full of gold bars

    //  Max Payne's State (the world's most depressed super-soldiers)
    addTile("Hoboken", "MAX", 2, 30, 1); // Rain never stops. Neither does the drinking
    addTile("São Paulo", "MAX", 2, 35, 1); // Luxury meets gunfire — a class war noir novel

    // Bullytown Education Board (religiously authoritarian school system)
    addTile("Bullworth Academy", "BLY", 3, 80); // Students rule the halls through violence and prayer
    addTile("Teacher Housing", "BLY", 1, 75); // Underpaid and overmilitarized

    // Rockstar Central (a parody of your own dev studio)
    addTile("Dev Floor", "RSC", 4, 85, 1); // Where crunch time never ends
    addTile("Online Lobby", "RSC", 3, 50, 1); // Chaotic energy field of griefers and patch bugs

    // Conflict Setup
    ledger["BLC"].atWarWith = "LSC"; // The rural insurgents have had enough of microtransactions
    ledger["LSC"].atWarWith = "BLC"; // Corporate-backed military retaliation campaign

    ledger["MAX"].atWarWith = "RSC"; // Noir soldiers declare war on the devs who ruined their sequel
    ledger["RSC"].atWarWith = "MAX"; // Noir soldiers declare war on the devs who ruined their sequel


    std::cout << "Grand Theft Geopolitics scenario loaded.\n";
    return selectPlayer();
}

// The special effects would be bad for computer to do(eat RAM)- The scenario nobody asked for but everybody needed
std::string loadTeletubbyScenario() {
    ledger.clear(); // Clearing reality for pure, unadulterated children's TV nightmare fuel
    game_map.clear(); // Tabula rasa, but make it pastel and deeply unsettling

    // The most cursed political simulation ever conceived by mortal minds
    ledger["TUB"] = Country("TUB", "Teletubbyland", GovernmentType::Theocracy, 20'000);//make the happiness not fall under 80 (OR THE SUN BABY GETS ANGRY)
    ledger["VAC"] = Country("VAC", "Noo Noo's Rebellion", GovernmentType::MilitaryJunta, 3'000);//+100 to armyEffective power (SUCTION POWER INTENSIFIES)
    ledger["SUN"] = Country("SUN", "Solar Baby Cult", GovernmentType::AbsoluteMonarchy, 4'000);//Gains 200 money if happiness >90 (baby giggles = economic prosperity)
    ledger["BPU"] = Country("BPU", "Bananaphone Union", GovernmentType::Communist, 15'000);//gain +100 if not at war;-300 if at war (peace dividends vs. dropped calls)
    ledger["DUM"] = Country("DUM", "Dumpling Hills", GovernmentType::Democracy, 1'000);// All tiles gain +5 happiness per tick if not at war (comfort food diplomacy)

    // TUB (where nightmares wear colorful costumes)
    addTile("Tubby Hill", "TUB", 3, 100, 2); // Maximum happiness or the eldritch television entities get upset
    addTile("Tubby Dome", "TUB", 2, 95, 2);  // Architectural horror made cheerful
    addTile("Giggle Fields", "TUB", 1, 90, 1); // Where joy goes to die and be reborn as something worse

    // VAC (the vacuum cleaner uprising nobody saw coming)
    addTile("Noo Noo Cave", "VAC", 1, 40, 3); // Underground lair of domestic appliance revolution
    addTile("Dust Storage", "VAC", 1, 50);     // The most boring strategic resource ever

    // SUN (celestial baby dictatorship)
    addTile("Babyface Temple", "SUN", 2, 85); // Where they worship the cosmic infant overlord
    addTile("Sunray Palace", "SUN", 1, 75);   // Solar-powered authoritarianism

    // BNN (telecommunications comedy hour)
    addTile("Bananaphone HQ", "BPU", 3, 70, 1);     // Corporate headquarters of fruit-based communication
    addTile("Call Center Alpha", "BPU", 2, 65, 2);  // Where customer service meets existential dread

    // DUM (the surprisingly wholesome democracy)
    addTile("Steamed Valley", "DUM", 2, 80, 3);   // Peaceful dumpling production
    addTile("Boiled Borough", "DUM", 2, 85);   // Different cooking method, same democratic values

    // War tension (because even in Teletubby land, conflict is inevitable)
    ledger["VAC"].atWarWith = "TUB"; // Machine vs. Tubby: the war nobody wanted
    ledger["TUB"].atWarWith = "VAC"; // When household appliances rebel against their colorful overlords

    std::cout << "Teletubbyland scenario loaded.\n"; 
    return selectPlayer(); // Choose your fighter in the most cursed political simulation ever created
}

bool NCRcitizenDeclaration(){
    std::string input;
    int attempts = 0;
    std::string reqNumber = "REQ-2077-00001";
    std::cout << "Assigned Requisition Number: REQ-2077-" << reqNumber << "\n";
    std::cout << “Please include this number in all future complaints, which will not be read.\n\n";
 

    std::cout << "\n=== NCR FORM 2077-A: CITIZENSHIP DECLARATION ===\n";
    std::cout << "NOTE: This form does not guarantee citizenship, nor the acknowledgment of this form.\n\n";
    std::cout << "Failure to comply results in forfeiture of democratic rights and preferred spawn locations.\n\n";

    inputPrint( "1. Please enter your full legal name: ");

    input = inputPrint( "2. Please enter your full NCR-issued Identity Confirmation Number (ICN): ");
    if (input.empty()) {
        std::cout << "ERROR: ICN cannot be empty unless authorized. Authorization requires valid ICN.\n";
        attempts++;
    }

    input=inputPrint("3. Choose your citizenship tier: [A] Aspirant, [B] Probationary, [C] Disavowed\n> ");
    std::cout << "Tier " << input << " selected. We regret to inform you this tier is currently not recognized.\n";

    inputPrint( "4. Please list any and all mutant DNA sequences you may have been exposed to (use N/A if unsure): ");

    inputPrint( "5. Estimated number of bottle caps earned last fiscal quarter (rounded to nearest dozen): ");
    std::cout << "Thank you. This number will be held against you during audit season.\n";

    inputPrint( "6. Please specify the number of dependents you’ve never had: ");
    std::cout << "Thank you. This number will be held against you during audit season.\n";

    input = inputPrint( "7. Have you previously declared undeclared assets you were unaware of? (yes/no/probably): ");
    if (input == "no") {
        std::cout << "Inconsistency detected: All citizens have undeclared assets. Honesty penalty applied.\n";
        attempts++;
    }

    input = inputPrint( "8. Are you currently harboring any pro-Legion sentiments? (yes/no/maybe)");
    if (input == "yes" || input == "maybe") {
        std::cout << "FLAGGED: Your response has been forwarded to the Office of Internal Loyalty.\n";
        attempts++;
    }

    input = inputPrint(  "9. Confirm that you have read and accepted the NCR Constitution, Volume III (yes/no): ");
    if (input != "yes") {
        std::cout << "Error: Constitution compliance is mandatory. Please recite at least one amendment:\n";
        std::getline(std::cin, input);
        std::cout << "Analysis complete. Confidence in democracy: 37%\n";//used to be random, but hardcoding 37% saves RAM
        attempts++;
    }

    input = inputPrint( "10. Confirm you have not read or understood Clause 69-B of Form 8008-T: ");
    if (input != "confirm") {
        std::cout << "Clause 19-B must be unknowable. You have now breached epistemological containment.\n";
        attempts += 2;
    }

    input = inputPrint("11. Declare your belief in the legitimacy of the NCR in no fewer than 50 characters: ");
    std::getline(std::cin, input);
    if (input.length() < 50) {
        std::cout << "Insufficient reverence detected. Patriotism threshold unmet.\n";
        attempts++;
    }

    input = inputPrint( "12. Are you aware that by participating in this form, you have already agreed to its terms retroactively? (yes): ");
    if (input != "yes") {
        std::cout << "Non-consent detected. Consent has been assumed. Resistance logged.\n";
        attempts++;
    }

   if (attempts > 2 {
     	std::cout << "\nYour answers have triggered a manual review by the NCR Bureau of Trustworthiness.\n";
        std::cout << "You may appeal this process by resubmitting Form 2077-X.\n";
       	std::cout << "Please wait 3 to 5 business months for appeal status.\n";
        std::cout << "Would you like to begin again? (yes/no): ";
        std::getline(std::cin, input);
        if (input == "yes") {
            return NCRcitizenDeclaration(); // Recursive, like true despair
        } else {
            std::cout << "\nYou have been silently disqualified. Your file has been sent to Archives.\n";
            return false;
        }
    }

    std::cout << "\nYour tax form has been accepted. You are now eligible to represent the glorious inefficiency of the NCR.\n";
    return true;
}
