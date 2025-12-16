/*

Main.cpp

finalized demo: Dec/15/2025
Executes everything from the game engine to make a game
User will have to load each scene

*/



#include <iostream>
#include "src/includes/scenes.cpp"


struct Settings
{
    bool autoContinueMonologues = false;
};

void firstBoot() {

    information("Checking save data");
    if (bootCheck("inventory.txt", "save.txt")) {
        information("Files in directory");
        return;
    }
    warning("First time boot: creating files");
    std::string files[2] = {"save.txt", "inventory.txt"};

    for (int i=0; i<2; i++) {
        std::ofstream file;
        file.open(files[i]);

        if (!file.is_open()) {
            warning("Could not create file");
        }

        file.close();
    
    }

}

/*

Game menu

1. Move
2. Goto shop
3. Select/use an item

*/

int gameMenu() {

    output("MENU SCREEN:");
    std::vector<std::string> options = {"Move", "Shop", "Items", "Player Stats", "Move to next stage", "quit"};
    return playerOptions(options);

}



/*

DUNGEON LAYERS

T = Top
M = Middle
L = Lower

CURRENTLY DEVELOPED:
T1 - Top layer 1

*/


void DungeonLayerT1(float min_health, float max_health, float attackMin, float attackMax, float key_price) {
    int userSelect;

    std::vector<Base_item*> empty = {};
    Player* player = loadPlayer();

    float health_type_weak = RandomNumber(min_health-1, max_health-1);
    float health_type_normal = RandomNumber(min_health, max_health);
    float health_type_strong = RandomNumber(min_health+3, max_health+3);

    float attack_type_weak = RandomNumber(attackMin-1, attackMax-1);
    float attack_type_normal = RandomNumber(attackMin, attackMax);
    float attack_type_strong = RandomNumber(attackMin+1, attackMax+1);



    Place SectionAA;
    SectionAA.locationName = "Starting point AA";
    SectionAA.description = "Starting point of this dungeon";
    //SectionAA.entitesHere.push_back(std::make_shared<Person>("Goblin", health_type_normal, 100.0f, 0.0f, {}, attack_type_normal, 2));
    SectionAA.entitesHere.push_back(std::make_shared<Person>("Goblin", health_type_normal, 100.0f, 0.0f, empty, attack_type_normal, 2));
    SectionAA.entitesHere.push_back(std::make_shared<Person>("Zombie", health_type_normal, 100.0f, 0.0f, empty, attack_type_strong, 2));


    Place SectionAB;
    SectionAB.locationName = "AB";
    SectionAB.description = "A place next to the starting point";
    SectionAB.entitesHere.push_back(std::make_shared<Person>("Vampire", health_type_strong, 100.0f, 0.0f, empty, attack_type_strong, 2));
    SectionAB.entitesHere.push_back(std::make_shared<Person>("Bat", health_type_weak, 100.0f, 0.0f, empty, attack_type_weak, 2));

    Place SectionBA;
    SectionBA.locationName = "BA";
    SectionBA.entitesHere.push_back(std::make_shared<Person>("Goblin", health_type_normal, 100.0f, 0.0f, empty, attack_type_normal, 2));
    SectionBA.entitesHere.push_back(std::make_shared<Person>("Zombie", health_type_normal, 100.0f, 0.0f, empty, attack_type_strong, 2));


    Place SectionBB;
    SectionBB.locationName = "BB";
    SectionBB.entitesHere.push_back(std::make_shared<Person>("Vampire", health_type_strong, 100.0f, 0.0f, empty, attack_type_strong, 2));
    SectionBB.entitesHere.push_back(std::make_shared<Person>("Bat", health_type_weak, 100.0f, 0.0f, empty, attack_type_weak, 2));

    SectionAA.compass["north"] = &SectionAB;
    SectionAA.compass["east"]  = &SectionBA;

    SectionAB.compass["south"] = &SectionAA;
    SectionAB.compass["east"]  = &SectionBB;

    SectionBA.compass["west"]  = &SectionAA;
    SectionBA.compass["north"] = &SectionBB;

    SectionBB.compass["west"]  = &SectionAB;
    SectionBB.compass["south"] = &SectionBA;


    Place* currentPlace = &SectionAA;


    while (true) {

          
        if (!player->isAlive()) {
            output("Player's health is less than 0");
            player->addHP(5);

        }
        
        std::vector<std::string> directions = {"north", "south", "east", "west"};
        userSelect = gameMenu();


        switch (userSelect) {

            case 1:

                player->think("Which direction should I go?");
                userSelect = playerOptions(directions);
                output("You chose to go " + directions[userSelect - 1]);
                move(currentPlace, directions[userSelect - 1]);


                if (Chance(1, 4)) {

                    int selected_Monster = RandomNumber(0, 1);
                    try {
                        fight(player, currentPlace->entitesHere[selected_Monster].get());
                    } catch (const EntityException& e) {
                        error(e.what());
                        output("Player has died, leaving safely");
                        return;
                    }
                    player->Save();
                }
 
                break;
            case 2: {
                Weapon* weapon = new Weapon("Stone_Sword", 20.00f, 50.00f);
                Food* food = new Food("Apple", 15.00f, 1.00f);
                Merchant* dungeonMerchant = new Merchant("Dungeon Merchant");
                dungeonMerchant->Pick_Up(weapon);
                dungeonMerchant->Pick_Up(food);
                Store(dungeonMerchant, player);
                delete dungeonMerchant;
                delete food;
                delete weapon;
                break;
            }

            case 3:
                player->inventoryMenu();
                break;
            
            case 4: {
                std::map<std::string, std::string> data = player->Profile();
                outputMap(data); 
                outputHalt("Press enter to continue");
                break;
            }
            case 5: 
                output("Attemping to level up");

                output("Key price: "+std::to_string(key_price));
                if (confirm()) {
                    if (player->getMoney() >= key_price) {
                        
                        player->removeMoney(key_price);
                        newScene(player);
                        delete player;
                        return; 
                    }

                    warning("Player can't buy the key");
                }
                break;
            
            case 6:
                if (confirm()) {
                    delete player;
                    output("Leaving");
                    return;
                }

        }
        

    

    }
}



class Game {


public:

    Settings settings;

    void settingsMenu() {
        output("Game Settings:");
        output("This currently does not do anything");
        std::vector<std::string> options = {
            "Toggle Auto-Continue Monologues"
        };
        int userSelect = playerOptions(options);
    }

    void Tutorial(int scene) {
       
        switch(scene) {
            case 1:
                ANewWorld();
                break;
            case 2:
                Beginning();
                break;
            case 3:
                APlayerHasAppeared();
                break;
            default:
                warning("Scene not found!");
                break;
        }

    }

    void DungeonCrawler(int layer) {
        if (layer <= 3) {
            DungeonLayerT1(10+layer, 45+layer, 1+layer, 2+layer, static_cast<float>(100*layer));
        } else {
            credits();
        }

    }

};


void startGame() {

    Game* game = new Game();
    while (true) {
        int scene = std::stoi(readCharacterFile("save.txt").scene);
        output("Loading scene " + std::to_string(scene));
        if (scene <= 3) {

            game->Tutorial(scene);

        } else {
            int layer = scene-3;
            game->DungeonCrawler(layer);
            break;
        }
    }
    delete game;
}

void debugScript() {

    Player* player = new Player("Johnny");

    player->addMoney(15);
    player->addMoney(15);


}

int main() {

    firstBoot();
    int userValue;
    output("Starting program...");
    sleep(1);

    do {
        output("An interesting game (demo)");
        userValue = menu();

        switch(userValue) {

            case 1:
                if (confirm()) {
                    newGame();
                }
                break;
            case 2: 
                startGame();
                break;
            case 3:
                Game game;
                game.settingsMenu();
                break;

    }

    } while (userValue != 4); 
    information("Loop has been closed");

    return 0;
}