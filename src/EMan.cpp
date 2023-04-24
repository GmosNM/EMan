#include "EMan.h"


namespace fs = std::experimental::filesystem;


void EMan::addPackage(std::string name){
    if (checkForPackage(name)) {
        createDirectory(name);
        cloneFromGithub(getSecondPackageLink(name), " packages\\" + name);
        updateEManFile();
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
    std::cout << "Enter the build system: ";
    std::cin >> buildSystem;
    setProjectName(projectName);
    createEManFile();
    
    std::ofstream cmakeFile(".\\CMakeLists.txt");
    cmakeFile << "cmake_minimum_required(VERSION 3.5)\n";
    cmakeFile << "\n# EMAN-PROJECT = " << projectName;
    cmakeFile << "\nproject(" << projectName << ")\n\n";
    cmakeFile << "set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n";
    cmakeFile << "\n";
    std::vector<std::string> files = getFilesInDirectory("./packages");
    for (const auto& file : files) {
        if (file == "glm") {
            cmakeFile << "";
        }else{
            cmakeFile << "\nadd_subdirectory(";
            cmakeFile << "\n# EMAN-PACKAGE = " << file;
            cmakeFile << "\n\tpackages/" << file << "";
            cmakeFile << "\n)";
        }
    }
    cmakeFile << "\n# EMAN-SRC";
    cmakeFile << "\n";
    std::string srcDir = "./src";
    if (fs::exists(srcDir)) {
        std::set<std::string> srcFiles = checkSourceDir(srcDir);
        cmakeFile << "set(SRC\n";
        for (const auto& file : srcFiles) {
            std::string fileName = file.substr(file.find_last_of("/\\") + 1);
            cmakeFile << "\t" << "src/" << fileName << "\n";
        }
        cmakeFile << ")\n\n";
    }

    cmakeFile << "add_executable(" << projectName << " " << "${SRC}" << ")\n";
    cmakeFile << "\n\ninclude_directories( " << projectName;
    for (const auto& file : files) {
        if (file == "glm") {
            cmakeFile << "\n# EMAN-PACKAGE = " << file;
            cmakeFile << "\n\t"<< "packages/" << file;
        }else{
            cmakeFile << "\n# EMAN-PACKAGE = " << file;
            cmakeFile << "\n\t"<< "packages/" << file << "/include";
        }
    }
    cmakeFile << "\n)";

    cmakeFile << "\n\ntarget_link_libraries( " << projectName;
    for (const auto& file : files) {
        if (file == "glm") {
            cmakeFile << "";
        }else{
            cmakeFile << "\n# EMAN-PACKAGE = " << file;
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

std::set<std::string> EMan::checkSourceDir(std::string directoryPath) {
    std::set<std::string> files;
    std::vector<std::string> allowedExtensions = {".cpp", ".c", ".h", ".hpp"};
    if (!fs::exists(directoryPath)) {
        std::cerr << "Directory does not exist: " << directoryPath << std::endl;
        return files;
    }
    for (const auto &entry : std::experimental::filesystem::directory_iterator(directoryPath)) {
        if (entry.status().type() == std::experimental::filesystem::file_type::regular) {
            std::string extension = entry.path().extension().string();
            if (std::find(allowedExtensions.begin(), allowedExtensions.end(), extension) != allowedExtensions.end()) {
                files.insert(entry.path().string());
            }
        }
    }
    return files;
}




std::vector<std::string> EMan::getFilesInDirectory(std::string directoryPath) {
    std::vector<std::string> files;
    if (!fs::exists(directoryPath)) {
        std::cerr << "Directory does not exist: " << directoryPath << std::endl;
        return files;
    }
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


void EMan::createEManFile(){
    std::ofstream emanFile(".\\.eman");
    std::vector<std::string> files = getFilesInDirectory("./packages");
    std::set<std::string> src_files = checkSourceDir("./src");
    std::string project = getProjectName();
    emanFile << "PROJECT :: " << project << "\n";
    for (const auto& file : files) {
        emanFile << "PACKAGE :: " << file << "\n";
    }
    emanFile << "EMAN-SRC ::\n"; 
    for (const auto& file : src_files) {
        std::string fileName = file.substr(file.find_last_of("/\\") + 1);
        std::string dirName = file.substr(0, file.find_last_of("/\\") + 1);
        emanFile << "\t" << dirName << fileName << "\n";
    }
    emanFile.close();
}



void EMan::setProjectName(std::string name){
    projectName = name;
}

std::string EMan::getProjectName(){
    return projectName;
}

void EMan::updateEManFile(){
    std::string emanFilePath = ".\\.eman";
    if (!fs::exists(emanFilePath)) {
        return;
    }
    std::vector<std::string> files = getFilesInDirectory("./packages");
    std::set<std::string> src_files = checkSourceDir("./src");
    std::ifstream cmakeFile(".\\CMakeLists.txt");
    std::string emanProjectLine;
    while (std::getline(cmakeFile, emanProjectLine)) {
        if (emanProjectLine.find("# EMAN-PROJECT =") != std::string::npos) {
            std::string emanProjectName = emanProjectLine.substr(emanProjectLine.find("=") + 2);
            std::ofstream emanFile(emanFilePath);
            setProjectName(emanFilePath);
            emanFile << "PROJECT :: " << emanProjectName << "\n";
            for (const auto& file : files) {
                emanFile << "PACKAGE :: " << file << "\n";
            }
            emanFile << "EMAN-SRC ::\n"; 
            for (const auto& file : src_files) {
                std::string fileName = file.substr(file.find_last_of("/\\") + 1);
                std::string dirName = file.substr(0, file.find_last_of("/\\") + 1);
                emanFile << "\t" << dirName << fileName << "\n";
            }
            emanFile.close();
            break;
        }
    }
    cmakeFile.close();
}

void EMan::updateCMakeFile(){
    std::string cmakeFilePath = "./CMakeLists.txt";
    if (!fs::exists(cmakeFilePath)) {
        return;
    }
    std::ifstream cmakeFile(cmakeFilePath);
    std::string line;
    std::stringstream updatedCmakeContents;
    std::set<std::string> existingFiles;
    std::vector<std::string> allowedExtensions = {".cpp", ".c", ".h", ".hpp"};
    bool setSrcFound = false;
    while (std::getline(cmakeFile, line)) {
        if (line.find("set(SRC") != std::string::npos) {
            if (!setSrcFound) {
                updatedCmakeContents << "set(SRC\n";
                std::set<std::string> src_files = checkSourceDir("./src");
                for (const auto& file : src_files) {
                    std::string fileName = file.substr(file.find_last_of("/\\") + 1);
                    std::string dirName = file.substr(0, file.find_last_of("/\\") + 1);
                    std::string filePath = dirName + fileName;
                    updatedCmakeContents << "\t" << "src/" << fileName << "\n";
                    existingFiles.insert(fileName);
                }
                setSrcFound = true;
            }
        } else {
            bool fileFound = false;
            for (const auto& ext : allowedExtensions) {
                if (line.find(ext) != std::string::npos) { 
                    std::string fileName = line.substr(line.find_last_of("/\\") + 1);
                    if (existingFiles.find(fileName) == existingFiles.end()) { 
                        existingFiles.insert(fileName);
                    } else {
                        fileFound = true;
                        break;
                    }
                }
            }
            if (!fileFound) {
                updatedCmakeContents << line << "\n";
            }
        }
    }
    if (!setSrcFound) {
        updatedCmakeContents << "set(SRC\n";
        std::set<std::string> src_files = checkSourceDir("./src");
        for (const auto& file : src_files) {
            std::string fileName = file.substr(file.find_last_of("/\\") + 1);
            std::string dirName = file.substr(0, file.find_last_of("/\\") + 1);
            std::string filePath = dirName + fileName;
            updatedCmakeContents << "\t" << "src/" << fileName << "\n";
            existingFiles.insert(fileName);
        }
    }
    for (const auto& ext : allowedExtensions) {
        std::set<std::string> files = checkSourceDir("./src");
        for (const auto& file : files) {
            std::string fileName = file.substr(file.find_last_of("/\\") + 1);
            if (existingFiles.find(fileName) == existingFiles.end()) {
                updatedCmakeContents << "add_executable(" << fileName.substr(0, fileName.find_last_of(".")) << " " << file << ")\n";
            }
        }
    }
    cmakeFile.close();
    std::ofstream updatedCmakeFile(cmakeFilePath);
    updatedCmakeFile << updatedCmakeContents.str();
    updatedCmakeFile.close();
}
