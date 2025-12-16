/*

Useful tools that aren't apart of the game but are useful


*/



#include <random>
#include <iostream>
#include <stdexcept>
#include "exceptions.cpp"
#include <filesystem>
#include <unistd.h>
#include <memory>
#include "logger.cpp"

std::random_device rd;
std::mt19937 gen(rd());
template <typename T>
T RandomNumber(T min, T max) {

    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);

}

bool Chance(int min, int max) {

    int number = RandomNumber(min, max);
    int select_number = RandomNumber(min, max);

    return number == select_number;


}

template <typename T> 
void numberValidation(T number, T max) {

    if (number > max) {
        throw EntityException("Number validation failed.");
    }

}


template <typename T, size_t N>
constexpr size_t arraySize(const T (&)[N]) {
    return N;
}


void sleep(int seconds) {
    usleep(seconds * 1000000);
}

template <typename T>
int playerOptions(std::vector<T>& options, bool show_items = true, bool leave = false) {
    int userSelect;
    int min_value = 1;
    if (leave) {
        min_value = 0;
    }

    if (show_items) outputVector(options);
    
    do {
        input("Select an option", userSelect);
    } while (userSelect < min_value || userSelect > static_cast<int>(options.size()));


    return userSelect;
}



bool bootCheck(std::string save_name, std::string inventory_name) {
    return (std::filesystem::exists(save_name) && std::filesystem::exists(inventory_name));
}


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