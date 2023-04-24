#include "EMan.h"


namespace fs = std::experimental::filesystem;


void EMan::addPackage(std::string name){
    if (checkForPackage(name)) {
        createDirectory(name);
        cloneFromGithub(getSecondPackageLink(name), " packages\\" + name);
    }
}


void EMan::createDirectory(std::string name){
    std::string dirPath = fs::current_path().string() + "\\" + "packages\\" + name;
    if (!fs::exists("packages")) {
        fs::create_directory("packages");
    }
    if (!fs::exists(dirPath)) {
        fs::create_directory(dirPath);
        std::cout << "Added package: " << name << std::endl;
    }
}

std::vector<std::pair<std::string, std::string>> EMan::getPackages(){
    return packages;
}


std::string EMan::getPackageVersion(std::string packagename){
    std::string packagePath = fs::current_path().string() + "\\" + "packages\\" + packagename;
    std::string versionPath = packagePath + "\\" + "version.txt";
    std::string version;
    if (fs::exists(versionPath)){
        std::ifstream versionFile(versionPath);
        std::getline(versionFile, version);
    }
    return version;
}

bool EMan::checkForPackage(std::string packageName){
    for (std::pair<std::string, std::string> package : packages){
        if (package.first == packageName){
            return true;
        }
    }
    std::cout << "Package not found!" << std::endl;
    return false;
}

void EMan::cloneFromGithub(std::string url, std::string filePath){
    std::string command = "git clone " + url + " " + filePath;
    if (system(command.c_str()) == -1) {
        std::cout << "You do not have sufficient privilege to perform this operation." << std::endl;
        return;
    }
}

std::string EMan::getSecondPackageLink(std::string packagename){
    for (int i = 0; i < packages.size(); i++) {
        if (packages[i].first == packagename) {
            if (i < packages.size()) {
                return packages[i].second;
            } else {
                return "ERROR: package not found";
            }
        }
    }
    return "";
}


void EMan::createCMakeFile(){
    std::string projectName;
    std::string mainFileLocation;
    std::string buildSystem;
    std::cout << "Enter project name: ";
    std::cin >> projectName;
    std::cout << "Enter main file location: ";
    std::cin >> mainFileLocation;
    std::cout << "Enter the build system: ";
    std::cin >> buildSystem;
    
    std::ofstream cmakeFile(".\\CMakeLists.txt");
    cmakeFile << "cmake_minimum_required(VERSION 3.5)\n";
    cmakeFile << "project(" << projectName << ")\n\n";
    cmakeFile << "set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n";
    cmakeFile << "\n";
    std::vector<std::string> files = getFilesInDirectory("./packages");
    for (const auto& file : files) {
        if (file == "glm") {
            cmakeFile << "";
        }else{
            cmakeFile << "\nadd_subdirectory(";
            cmakeFile << "\n\tpackages/" << file << "";
            cmakeFile << "\n)";
        }
    }
    cmakeFile << "\n\n" << "set(SRC\n \t " << mainFileLocation << "\n)\n\n";
    cmakeFile << "add_executable(" << projectName << " " << "${SRC}" << ")\n";
    cmakeFile << "\n\ninclude_directories( " << projectName;
    for (const auto& file : files) {
        if (file == "glm") {
            cmakeFile << "\n\t"<< "packages/" << file;
        }else{
            cmakeFile << "\n\t"<< "packages/" << file << "/include";
        }
    }
    cmakeFile << "\n)";

    cmakeFile << "\n\ntarget_link_libraries( " << projectName;
    for (const auto& file : files) {
        if (file == "glm") {
            cmakeFile << "";
        }else{
            cmakeFile << "\n\t" << file;
        }
    }
    cmakeFile << "\n)";
    std::string command = "cmake -G " + buildSystem + " -B bld";
    if (system(command.c_str()) == -1) {
        std::cout << "You do not have sufficient privilege to perform this operation." << std::endl;
        return;
    }
}


std::vector<std::string> EMan::getFilesInDirectory(std::string directoryPath) {
    std::vector<std::string> files;
    for (const auto &entry : std::experimental::filesystem::directory_iterator(directoryPath)) {
        if (entry.status().type() == std::experimental::filesystem::file_type::directory) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}


void EMan::build(){
    std::string command = "cmake --build bld";
    if (system(command.c_str()) == -1) {
        std::cout << "You do not have sufficient privilege to perform this operation." << std::endl;
        return;
    }
    std::string command2 = "mv bld/compile_commands.json ./compile_commands.json";
    std::string compile_commands_path = "bld/compile_commands.json";
    if (!fs::exists(compile_commands_path)) {
        return;
    }
    system(command2.c_str()); 
}
