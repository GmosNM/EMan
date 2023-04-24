#include "EMan.h"

// Function to print usage instructions
void printUsage() {
    std::cout << "Usage: PKG-Manager [command] [package name]" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "add\t\tAdd a package" << std::endl;
    std::cout << "remove\t\tRemove a package" << std::endl;
    std::cout << "update\t\tUpdate a package" << std::endl;
    std::cout << "showPackages\tShow all packages" << std::endl;
    std::cout << "cmake\tCreates a cmake file in the same directory" << std::endl; // Added command to show all packages
    std::cout << "build\t\tBuild the cmake file if found" << std::endl; // Added command to build a package
}

int main(int argc, char* argv[]) {
    EMan manager;
    if (argc >= 2) {
        std::string command = argv[1];
        if (command == "add" && argc == 3) {
            manager.addPackage(argv[2]); // Added package version to addPackage function call
        } else if (command == "remove" && argc == 3) {
            //manager.removePackage(argv[2]);
        } else if (command == "update") {
            //manager.updatePackage(argv[2]);
        } else if (command == "cmake") {
            manager.createCMakeFile();
        } else if (command == "showPackages") { // Added command to show all packages
            std::vector<std::pair<std::string, std::string>> packages = manager.getPackages();
            for (const auto& package : packages) {
                    std::cout << package.first << std::endl;
            }

        } else if (command == "version" && argc == 3){
            std::string packageVersion = manager.getPackageVersion(argv[2]); // Added package version to getPackageVersion function call
            std::cout << packageVersion << std::endl;
        } else if (command == "build") { // Added command to build a package
            manager.build();
        } else if (command == "--help") {
            printUsage(); // Call the printUsage function if the user enters the --help command
        } else {
            std::cout << "Invalid command. Please use add, remove, update, version, showPackages, or build." << std::endl; // Updated error message
        }
    } else {
        std::cout << "Invalid command. Please use add, remove, update, version, showPackages, or build." << std::endl; // Updated error message
    }
    return 0;
}

