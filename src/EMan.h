#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <set>
#include <sstream>


class EMan {
public:
    void addPackage(std::string);
    void removePackage(std::string);
    void createDirectory(std::string);
    bool checkForPackage(std::string packageName);
    std::vector<std::pair<std::string, std::string>> getPackages();
    std::string getPackageVersion(std::string packagename);
    void cloneFromGithub(std::string url, std::string filePath);
    std::string getSecondPackageLink(std::string packagename);
    void createCMakeFile();
    std::vector<std::string> getFilesInDirectory(std::string directoryPath);
    void build();
    void createEManFile();
    void setProjectName(std::string name);
    std::string getProjectName();
    void updateEManFile();
    std::set<std::string> checkSourceDir(std::string directoryPath);
    void updateCMakeFile();

private:
    std::string projectName;
    std::vector<std::pair<std::string, std::string>> packages = {
            {"glfw", "https://github.com/glfw/glfw"},
            {"sdl2", "https://github.com/libsdl-org/SDL"},
            {"raylib", "https://github.com/raysan5/raylib"},
            {"glad", "https://github.com/GmosNM/glad"},
            {"glm", "https://github.com/g-truc/glm"},
    };
    std::vector<std::string> needInclude;
    std::vector<std::string> needLib;
    std::vector<std::string> needAddSubdir;
};

