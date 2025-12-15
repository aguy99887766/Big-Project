/*

Cut scenes
Pre made cut scenes I can execute at anytime


*/



#include <iostream>
#include "engine/save.cpp"
#include "engine/rpgactions.cpp"


void newGame() {
    Person* mysteriousPerson = new Person("Mysterious Man");
    wipeFile("inventory.txt");
    std::string name = dialog<std::string>(mysteriousPerson, "Hello, what is you name");
    generateCharacter("save.txt", name);

    delete mysteriousPerson;

}

void Beta() {

    Person* owner = new Person("Owner");
    monologue(owner, "I am still experimenting with stuff");
    monologue(owner, "And this is the first real game I've made");
    monologue(owner, "So expect some bugs");
    monologue(owner, "This is meant to be more of a game engine to create games on");
    monologue(owner, "But this is a demo I created with what I've built thus far");

    delete owner;
}


void ANewWorld() {

    Player* player = loadPlayer();

    Person* mysteriousPerson = new Person("Mysterious Man");
    monologue(mysteriousPerson, "You are in a dungeon you must find your way out!");
    monologue(mysteriousPerson, "I will summon a monster for you to fight!");

    Person* monster = new Person("Zombie", 25.0f);

    monologue(mysteriousPerson, "Don't worry here is a sword");

    Weapon* sword = new Weapon("Wooden_Sword", 5.00f);

    player->Pick_Up(sword);
    player->selectWeapon(0);
    monologue(mysteriousPerson, "You're good, wake up now.");
    fight(player, monster);

    newScene(player);

    delete monster;
    delete sword;
    delete player;
    delete mysteriousPerson;


}




void Beginning() {

    Player* player = loadPlayer();


    Place startingPlace;
    startingPlace.locationName = "Dungeon Entrance";
    startingPlace.description = "A dark and damp dungeon entrance.";
        
    Place dungeonCorridor;
    dungeonCorridor.locationName = "Dungeon Corridor";
    dungeonCorridor.description = "A narrow corridor with flickering torches on the walls.";
    dungeonCorridor.entitesHere.push_back(std::make_shared<Person>("Goblin", 30.0f));
    dungeonCorridor.entitesHere.push_back(std::make_shared<Person>("Zombie", 45.0f));


    startingPlace.compass["north"] = &dungeonCorridor;
    dungeonCorridor.compass["south"] = &startingPlace;


    Place* currentPlace = &startingPlace;
    player->think("I need to find a way out of here.");
    monologue(player, "Where am I?");

    std::vector<std::string> directions = {"north", "south", "east", "west"};
    player->think("Which direction should I go?");
        
    int userSelect = playerOptions(directions);
    output("You chose to go " + directions[userSelect - 1]);

    move(currentPlace, directions[userSelect - 1]);

    fight(player, dungeonCorridor.entitesHere[0].get());

    newScene(player);
    delete player;

}

void APlayerHasAppeared() {

    Player* player = loadPlayer();
    Merchant* merchant = new Merchant("Wandering Trader");
    merchant->Pick_Up(new Weapon("Iron_Dagger", 7.0f, 30.0f));
    merchant->Pick_Up(new Food("Bread", 20.0f));

    player->think("I got out out alive");
    monologue(player, "What did I just fight?");

    Place DarkHall;
    DarkHall.locationName = "Dark Hall";
    DarkHall.description = "A dark hall with shadows lurking in the corners.";

    Store(merchant, player);

    monologue(merchant, "You don't seem to be from here");
    monologue(player, "Yeah, I am not from here what is this?");
    monologue(merchant, "This is called 'The Simulation'");
    monologue(player, "What am I suppose to do here?");
    monologue(merchant, "See how far you're able to make it");
    monologue(merchant, "Right after the user has decided to press enter the scene will switch and you will teleport");


    newScene(player);
    delete player;

}



void credits() {
    output("=== Credits ===");
    output("Game developed by A Guy.");
    output("Thanks for playing the demo!");
    output("================");

    outputHalt("Press enter to exit.");
    exit(0);
}

void debugScene() {

    Player* player = loadPlayer();
    player->addMoney(15);
    player->addMoney(12);
    delete player;
}

void loadGame() {
    ANewWorld();
}

bool confirm() {

    std::string confirm;

    do {
        input("Are you sure? (y/n)", confirm);

    } while (confirm != "y" && confirm != "n");

    return confirm == "y";

}

int menu() {
    std::string selectOption[4] = {
        "New Game",
        "Load Game",
        "Settings",
        "Exit"
    };
    int userChoice;
    outputArray(selectOption);
    input("Response", userChoice);
    return userChoice;

    
}