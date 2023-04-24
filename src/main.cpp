#include "EMan.h"
#include <iostream>

void printUsage() {
    std::cout << "Usage: PKG-Manager [command] [package name]" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "add\t\tAdd a package" << std::endl;
    std::cout << "remove\t\tRemove a package" << std::endl;
    std::cout << "update\t\tUpdate a package" << std::endl;
    std::cout << "showPackages\tShow all packages" << std::endl;
    std::cout << "cmake\tCreates a cmake file in the same directory" << std::endl;
    std::cout << "build\t\tBuild the cmake file if found" << std::endl;
}

int main(int argc, char* argv[]) {
    EMan manager;
    if (argc >= 2) {
        std::string command = argv[1];
        if (command == "add" && argc == 3) {
            manager.addPackage(argv[2]);
        } else if (command == "remove") {

        } else if (command == "update") {
            manager.updateCMakeFile();
            manager.updateEManFile();
        } else if (command == "cmake") {
            manager.createCMakeFile();
        } else if (command == "showPackages") {
            std::vector<std::pair<std::string, std::string>> packages = manager.getPackages();
            for (const auto& package : packages) {
                    std::cout << package.first << std::endl;
            }
        } else if (command == "version" && argc == 3){
            std::string packageVersion = manager.getPackageVersion(argv[2]);
            std::cout << packageVersion << std::endl;
        } else if (command == "build") {
            manager.build();
        } else if (command == "--help") {
            printUsage();
        } else {
            std::cout << "Invalid command. Please use add, remove, update, version, showPackages, or build." << std::endl;
        }
    } else {
        std::cout << "Invalid command. Please use add, remove, update, version, showPackages, or build." << std::endl;
    }
    return 0;
}

