#include <dirent.h>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include "fileio.hpp"

std::string CFileIO::saveFolder = "examples/savegame/";
std::string CFileIO::dataFolder = "examples/data/";

CFileIO::CFileIO() {
}

CFileIO::~CFileIO() {
}

bool CFileIO::Save(std::string filename, std::vector<std::string> data) const {
    std::ofstream file(filename, std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    for (std::string & line : data) {
        file << line << std::endl;
        if (file.fail()) {
            file.close();
            return false;
        }
    }
    file.close();

    return true;
}

std::vector<std::string> CFileIO::Load(std::string filename) const {
    std::vector<std::string> res;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return res;
    }
    std::string line;
    while (std::getline(file, line)) {
        res.push_back(line);
    }

    return res;
}

std::vector<std::string> CFileIO::GetSavedGamesFilenames() const {
    DIR * savedir;
    struct dirent * directory;
    std::vector<std::string> names;
    savedir = opendir(saveFolder.c_str());
    if (savedir) {
        while ((directory = readdir(savedir)) != NULL) {

            if (strncmp(directory->d_name, ".", strlen(directory->d_name)) != 0 && strncmp(directory->d_name, "..", strlen(directory->d_name)) != 0)
                names.push_back(directory->d_name);
        }

        closedir(savedir);
    }
    std::sort(names.begin(), names.end(), std::greater<std::string>());
    return names;
}

bool CFileIO::Delete(std::string filename) const {
    return remove(filename.c_str()) != 0;
}


