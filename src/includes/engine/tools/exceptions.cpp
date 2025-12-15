/*

Custom exeception for entities

*/


#include <iostream>
#include <exception>

class EntityException : public std::exception {
private:
    std::string message;

public:

    explicit EntityException(const std::string& reason) : message("Fatal entity exception has occured: " + reason) {}
    const char* what() const noexcept override {
        return message.c_str();
    }

};