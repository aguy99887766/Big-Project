#include "objects.cpp"


struct Place {

    std::string locationName;
    std::string description;


    std::map<std::string, Place*> compass;

    std::vector<std::shared_ptr<Person>> entitesHere;

};

void move(Place*& currentPlace, const std::string direction) {

    if (currentPlace->compass.find(direction) != currentPlace->compass.end()) {
        currentPlace = currentPlace->compass[direction];
        output("You moved to " + currentPlace->locationName);
        output(currentPlace->description);
        outputHalt("Press enter to continue");
    } else {
        warning("You can't go that way!");
    }

}

void monologue(Person* person, std::string message, bool autoContinue = false) {
    person->speak(message);
    if (autoContinue) {
        sleep(1);
    } else {
        outputHalt("Press enter to continue");
    }
}

template <typename T>
T dialog(Person* person, const std::string message, const std::vector<std::string>& responses = {}) {
    T value;
    person->speak(message);
    if (!responses.empty()) outputVector(responses);
    input("Response", value);

    return value;
}


Player* loadPlayer() {
    Chara saveData = readCharacterFile("save.txt");
    Player* user = new Player(saveData.name, std::stof(saveData.health), 100.0f, std::stof(saveData.money));
    user->loadInventory();
    return user;
}

void newScene(Player* player) {
    Chara saveData = readCharacterFile("save.txt");

    saveGame("save.txt",
             std::to_string(player->getLevel()),
             std::to_string(player->getMoney()),
            std::to_string(player->getHP()));
    updateCharacterScene("save.txt", std::to_string(std::stoi(saveData.scene) + 1));
    //saveData.scene = std::to_string(std::stoi(saveData.scene) + 1);
    //output("Updating scene game...");
    //writeCharacter("save.txt", saveData);
}

void Store(Merchant* merchant, Person* buyer) {
    output("MONEY: $"+std::to_string(buyer->getMoney()));
    merchant->speak("Welcome to my shop! Take a look around");
    merchant->listItems();
    std::vector<Base_item*> items = merchant->getInventory();

    int choice = playerOptions(items, false, true);
    if (choice == 0) return;
    merchant->buyItem(buyer, choice-1);
}

void GameOver() {
    error("That was disapointing...");
    output("Game over");
}

void fight(Player* player, Person* monster) {
    
    float baseHP = monster->getHP();

    output("You get into a fight with a " + monster->getName());
    outputHalt("Press enter to continue");
    do {    
        std::vector<std::string> fightOptions = {
            "Fight",
            "Get information",
            "Items",
            "Run"
        };

        output("Player HP: " + std::to_string(player->getHP()));
        output("Monster HP: " + std::to_string(monster->getHP()));

        player->think("Whats my next move?");
        int userSelect = playerOptions(fightOptions);

        switch(userSelect) {
            case 1:
                player->attack(monster);
                break;
            case 2: {
                std::map<std::string, std::string> data = monster->Profile();
                outputMap(data); 
                outputHalt("Press enter to continue");
                break;

            } 
            case 3: 
                player->inventoryMenu();
                break;
            case 4:
                if (Chance(1, 5)) {
                    output(player->getName()+" has fled");
                    return;
                }
                output("Failed to flee");
                break;
            default: 

                warning("Invalid option");
            
        }



    monster->attack(player);

    if (!player->isAlive()) {
        delete player;
        throw EntityException("Game Over");
        //GameOver();
        
    }

    } while (monster->getHP() > 0);

    monster->addHP(baseHP);


    output("You win!");
    player->addMoney(15);


}