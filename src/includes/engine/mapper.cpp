/*

Inventory manager for the player
Syncs the inventory from the player vector to a file


Has caused some crashes

To add an item from the text file the format is:
POS NAME IMPACT PRICE ID ITEM TYPE

*/



#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>



struct ItemTemplate {

    std::string position;
    std::string name;
    std::string impact;
    std::string price;
    std::string ID;
    std::string ItemType;

};

std::vector<ItemTemplate> ReadAttributes(std::string filename) {

    std::ifstream inputFile(filename);


    std::vector<ItemTemplate> items;

    if (!inputFile.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return {}; 
    }

    std::string attribute;

    while (std::getline(inputFile, attribute)) {

        if (attribute.empty() || attribute[0] == '#') continue;


        std::istringstream iss(attribute);

        ItemTemplate item;

        if (!(iss >> item.position >> item.name >> item.impact >> item.price >> item.ID >> item.ItemType)) {
            std::cerr << "Error line: " << attribute << std::endl;
            continue;
        }

        items.push_back(item);

    }

    return items;


}

void AddItem(std::string filename, std::string position, std::string name, std::string impact, std::string price, std::string ID, std::string ItemType) {


    std::ofstream outputFile(filename, std::ios::app);

    if (!outputFile.is_open()) {
        std::cerr << "Could not find " << filename << std::endl;
        return;
    }

    outputFile << position << " " << name << " " << impact << " " << price << " " << ID << " " << ItemType << std::endl;

    outputFile.close();
    std::cout << "Item added: " << name << std::endl;
}

void write(std::string filename, std::string line) {
    std::ofstream outputFile(filename, std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Could not find " << filename << std::endl;
        return;
    }

    outputFile << line << std::endl;
    outputFile.close();
}

void wipeFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);
}

void RemoveItem(std::string filename, int position) {
    std::ifstream readFile(filename);


    if (!readFile.is_open()) {
        std::cerr << "Could not find " << filename << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    int current = 1;

    while(std::getline(readFile, line)) {
        if (current != position) {
            lines.push_back(line);
        }
        current++;
    }

    readFile.close();
    //outputVector(lines);

    std::ofstream writeFile(filename, std::ios::trunc);

    for (const auto& line : lines) {
        writeFile << line << std::endl;
    }
    writeFile.close();
}

bool inVectorID(const std::vector<ItemTemplate>& neededVec, const std::string& item_find) {
    return std::find_if(neededVec.begin(), neededVec.end(), [&](const ItemTemplate item) {return item.ID == item_find;}) != neededVec.end();
}