/*

Objects that can be spawned into the game

*/



#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "tools/algorithms.cpp"
#include "mapper.cpp"


/*

Unused variables:
    Durability
    Hunger

*/

class Person;

// ==============================================
//                   BASE ITEM
// ==============================================
class Base_item {
protected:
    std::string name = "Generic Item";
    float durability = 100.0f;
    float impact = 0.0f;
    std::string type = "GENERIC";
    float price = 100.0f;

public:
    Base_item(std::string n, float im=15.00f, float p=100.0f, std::string t="GENERIC") : name(n), impact(im), price(p), type(t) {}
    virtual ~Base_item() = default;

    virtual void use(Person* user) = 0;
    std::string getName() const { return name; }
    
    float getImpact() const { return impact; }

    float getDurability() const { return durability; }
    float getPrice() const { return price; }
    std::string getItemType() const { return type; }
};

// ==============================================
//                    PERSON
// ==============================================
class Person {
protected:
    std::string name;
    std::vector<Base_item*> inventory_items = {};
    Base_item* selected_weapon = nullptr;

    float health;
    float hunger;
    float money;
    float punchDmg;
    int atkRange;

public:
    Person(std::string _name, float _health = 100.0f, float _hunger = 100.0f, float _money = 0.0f, std::vector<Base_item*> _items = {}, float _punchDmg = 1.0f, int _atkRange = 2)
        : name(_name), health(_health), hunger(_hunger), money(_money), inventory_items(_items), punchDmg(_punchDmg), atkRange(_atkRange)
    {
        for (auto &i : inventory_items)
            i = nullptr;
    }

    // ---------------------- INVENTORY ----------------------
    virtual void Pick_Up(Base_item* item) {
        inventory_items.push_back(item);
        
    }

    void useItem(int index) {
        if (index < 0 || index >= 15) {
            warning("Invalid inventory index!");
            return;
        }

        Base_item* item = inventory_items[index];
        if (!item) {
            warning("No item in that slot");
            return;
        }

        item->use(this); 
        inventory_items[index] = nullptr; 
    }

    void removeItem(int index) {
        output(inventory_items[index]->getName()+" has been removed from inventory");
        inventory_items.erase(inventory_items.begin() + index);
    }

    bool isAlive() {
        return health>0;
    }

    Base_item* getHand(int index) {
        if (index < 0 || index >= 15) return nullptr;
        return inventory_items[index];
    }

    void selectWeapon(int index) {
        if (index < 0 || index >= 15) return;
        selected_weapon = inventory_items[index];
    }

    void clear() {
        for (auto* item : inventory_items) {
            delete item;
        }
        inventory_items.clear();
    }

    void eat(float nutrition) {
        health += nutrition;
        if (health > 100.0f) hunger = 100.0f;
        information(name + " eats.");
    }

    void takeDamage(float dmg) {

        health -= dmg;
        information(name + " takes " + std::to_string(dmg) + " damage.");
        sleep(1);
        if (health < 0) health = 0;
    }

    void punch(Person* target) {
        float dmg = RandomNumber(punchDmg, punchDmg+atkRange);
        information(name + " punches!");
        target->takeDamage(dmg);
    }

    std::vector<Base_item*> getInventory() const {
        return inventory_items;
    }

    float getMoney() const { return money; }
    std::string getName() const { return name; }

    void addMoney(float amount) {
        output(name + " received $" + std::to_string(amount));
        money += amount;
        output("New balance: $" + std::to_string(money));
    }

    void removeMoney(float amount) {
        if (amount > money) {
            warning("Not enough money.");
            return;
        }
        money -= amount;
    }

    void transferMoney(Person* receiver, float amount) {
        if (amount > money) {
            warning("Not enough money to transfer.");
            return;
        }
        money -= amount;
        receiver->addMoney(amount);
    }

    void speak(std::string msg) {
        output(name + ": " + msg);
    }
    void addHP(float increment) {
        health += increment;
    }

    float getHP() const { return health; }
    float getHunger() const { return hunger; }
    float getMoneyAmount() const { return money; }
    // ---------------------- COMBAT ----------------------
    void attack(Person* target);

    std::map<std::string, std::string> Profile() {
        return {
            {"Health", std::to_string(health)},
            {"Hunger", std::to_string(hunger)},
            {"Money", std::to_string(money)}
        };
    }
};

class Food : public Base_item {


public:
    Food(std::string n, float nutrition, float p=15.00f) : Base_item(n, nutrition, p, "FOOD") {}

    void use(Person* user) override {

        user->eat(getImpact());
        output(user->getName()+" has eaten "+getName());
        outputHalt("Press enter to continue");
        delete this;

    }
};

// ==============================================
//                    WEAPON
// ==============================================
class Weapon : public Base_item {

public:
    Weapon(std::string n, float attack=15.00f, float p=15.00f)
        : Base_item(n, attack, p, "WEAPON") {}

    void use(Person* user) override { }

    bool use(Person* user, Person* target) {
        float dmg = RandomNumber(getImpact() - 4, getImpact() + 4);
        target->takeDamage(dmg);

        durability -= 2;

        if (durability <= 0) {
            information(name + " breaks!");
            delete this; 
            return true;
        }

        return false;
    }

};

// ==============================================
//          PERSON: ATTACK IMPLEMENTATION
// ==============================================
void Person::attack(Person* target) {
    if (!selected_weapon) {
        punch(target);
        return;
    }

    Weapon* w = dynamic_cast<Weapon*>(selected_weapon);

    if (!w) {
        warning("Selected item is not a weapon.");
        return;
    }

    bool broke = w->use(this, target); // weapon may delete itself

    if (broke) {
        for (int i = 0; i < 15; i++) {
            if (inventory_items[i] == selected_weapon) {
                inventory_items[i] = nullptr; // clear pointer
                selected_weapon = nullptr;
                break;
            }
        }
    }
}

class Merchant : public Person {

public:
    Merchant(std::string n, const std::vector<Base_item*>& inv = {})
        : Person(n, 100.0f, 100.0f, 0.0f, inv) {}

    
    void buyItem(Person* buyer, int item_choice) {
        std::vector<Base_item*> inventory = getInventory();
        Base_item* item = inventory[item_choice];
        float price = item->getPrice();


        if (buyer->getMoney() < price) {
            warning("Buyer doesn't have enough money.");
            return;
        }

        buyer->transferMoney(this, price);
        buyer->Pick_Up(item);
        removeItem(item_choice);
        output(buyer->getName()+" has bought "+item->getName());
    }


    void listItems() const {
        output("Items:");
        for (auto* item : inventory_items) {
            output("- " + item->getName() + " ( $"+ std::to_string(item->getPrice()) +") ");
        }
    }
};

class Player : public Person {
private:
    int level = 0;
    int currentDungeon = 0;

public:
    Player(std::string _name, float _health = 100.0f, float _hunger = 100.0f, float _money = 100.0f)
        : Person(_name, _health, _hunger, _money) {}

    void levelUp() {
        level++;
        output("You leveled up! Level = " + std::to_string(level));
    }

    void moveDungeon() {
        currentDungeon++;
    }

    int getDungeon() const {
        return currentDungeon;
    }

    int getLevel() const { return level; }

    void think(std::string msg) {
        output("(Thinking) " + name + ": " + msg);
    }

    void Save() {
        Chara saveData = readCharacterFile("save.txt");

        saveGame("save.txt",
             name,
             std::to_string(level),
             std::to_string(money),
             std::to_string(health)
        );
        output("Character data saved...");
       
    }

    void outputInventory(const std::vector<ItemTemplate> items) {

        for (size_t i = 0; i < items.size(); i++) {
            ItemTemplate item = items[i];
            output(std::string(25, '-'));
            output("POS: "+std::to_string(i+1));
            output("NAME: "+item.name);
            output("PRICE: $"+item.price);
            output("IMPACT: "+item.impact);
            output("TYPE: "+item.ItemType);
            output(std::string(25, '-'));
        }
    }

    virtual void Pick_Up(Base_item* item) override {
        addToInventory(item);
        updateInventory();
    }

    void loadInventory() {

        std::vector<ItemTemplate> items = ReadAttributes("inventory.txt");

        for (ItemTemplate item : items) {
            // output(item.ItemType);
            if (item.ItemType == "FOOD") {
                Food* player_item = new Food(item.name, std::stof(item.impact), std::stof(item.price));

                inventory_items.push_back(player_item);
            } else if (item.ItemType == "WEAPON") {
                Weapon* player_item = new Weapon(item.name, std::stof(item.impact), std::stof(item.price));
                inventory_items.push_back(player_item);
            } 


        }

    }

    void updateInventory() {
        clear();
        loadInventory();
    }

    void removeFromInventory(int position) {

        inventory_items.erase(inventory_items.begin() + position - 1);
        RemoveItem("inventory.txt", position); 

    }
    void inventoryMenu() {


        std::vector<ItemTemplate> items = ReadAttributes("inventory.txt");

        if (items.size() <= 0) {
            output(name+" has no items");
            return;
        }

        int userValue;
        output(name+"'s Inventory");
        outputInventory(items);  

        do {
            input("Select a value", userValue);

        } while (userValue < 1 || userValue > items.size());

        
        Base_item* selected_item = inventory_items[userValue - 1];

        if (!selected_item) {
            warning("No item detected");
            return;
        }
        if (items[userValue - 1].ItemType == "FOOD") {
            selected_item->use(this);  // Use the food item
            inventory_items[userValue - 1] = nullptr;  // Remove item from inventory
            removeFromInventory(userValue);  // Remove item from file
        } else if (items[userValue - 1].ItemType == "WEAPON") {
            selectWeapon(userValue - 1);  // Equip the weapon
        } 
        


    }

    void addToInventory(Base_item* item) {
        int inventory_size = inventory_items.size();
        AddItem("inventory.txt", std::to_string(inventory_size), item->getName(), std::to_string(item->getImpact()), std::to_string(item->getPrice()), "NULL", item->getItemType());
    }

};
