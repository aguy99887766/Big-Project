/*

    Save files
    Used for saving and loading a file
    

    Created by me & created with the help of AI

    This saves and load the player stats

    THE FORMAT IS
    NAME
    HEALTH
    SCENE
    MONEY

*/



#include <fstream>
#include <string>
#include <iostream>

struct Chara {
    std::string name;
    std::string health;
    std::string level;  // unused
    std::string scene;
    std::string currentWeapon;  // unused
    std::string money;
};



void writeCharacter(const std::string& filename, const Chara& character) {
    std::ofstream WriteFile(filename, std::ios::trunc); 
    if (WriteFile.is_open()) {
        WriteFile << character.name << std::endl;
        WriteFile << character.health << std::endl;
        WriteFile << character.level << std::endl;
        WriteFile << character.scene << std::endl;
        WriteFile << character.money << std::endl;
        WriteFile.close();
    } else {
        std::cerr << "Could not write to file." << std::endl;
    }
}

void generateCharacter(const std::string& filename, std::string name) {
    Chara character;
    character.name = name;
    character.health = "100.000000";
    character.level = "1";
    character.scene = "1";
    character.money = "0";
    writeCharacter(filename, character);
}

Chara readCharacterFile(const std::string& filename) {
    std::ifstream ReadFile(filename);
    Chara character;

    if (ReadFile.is_open()) {
        getline(ReadFile, character.name);  
        getline(ReadFile, character.health);
        getline(ReadFile, character.level); 
        getline(ReadFile, character.scene);
        getline(ReadFile, character.money);
        ReadFile.close();
    } else {
        std::cerr << "Could not read the file." << std::endl;
    }

    return character;  
}

void updateCharacterName(const std::string& filename, const std::string& newName) {
    Chara character = readCharacterFile(filename);
    character.name = newName;
    writeCharacter(filename, character);
} 

void updateCharacterLevel(const std::string& filename, const std::string& level) {
    Chara character = readCharacterFile(filename); 
    character.level = level; 
    writeCharacter(filename, character); 
}

void updateCharacterMoney(const std::string& filename, const std::string& newMoney) {
    Chara character = readCharacterFile(filename);
    character.money = newMoney;
    writeCharacter(filename, character);
}

void updateCharacterScene(const std::string& filename, const std::string& newScene) {
    Chara character = readCharacterFile(filename);
    character.scene = newScene;
    writeCharacter(filename, character);
}

void updateCharacterHealth(const std::string& filename, const std::string& newHealth) {
    Chara character = readCharacterFile(filename);
    character.health = newHealth;
    writeCharacter(filename, character);
}


void saveGame(const std::string& filename, const std::string& name, const std::string& level, const std::string& money, const std::string& health) {
    updateCharacterName(filename, name);
    updateCharacterLevel(filename, level);
    updateCharacterMoney(filename, money);
    updateCharacterHealth(filename, health);

}



