#include "includes/engine/save.cpp"
#include "includes/engine/rpgactions.cpp"


void Fight_Scene() {
    
    std::vector<Base_item*> empty = {};

    Player* player = loadPlayer();
    Person* monster = new Person("Goblin", 10.0f, 100.0f, 0.0f, empty, 10.0f, 2);
    fight(player, monster);
    player->Save();
    delete player;
    delete monster;
}


void Test_Scene() {
    Player* player = loadPlayer();
    player->think("I see a shop");
    monologue(player, "What items are there?");

    Merchant* merchant = new Merchant("Merchant");
    Food* peanuts = new Food("Peanuts",15.00f,1.00f);
    Food* apple = new Food("Apple", 5.00f, 5.00f);
    merchant->Pick_Up(peanuts);
    merchant->Pick_Up(apple);
    Store(merchant, player);
    
    delete player;
    delete merchant;
    delete apple;
    delete peanuts;
}


int main() {
    firstBoot();
    Fight_Scene();
    
    return 0;
}