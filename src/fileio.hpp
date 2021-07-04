#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <vector>
#include <string>
#include "gameBase.hpp"

/**
 * Performs filesystem operations - saving and loading.
 */
class CFileIO {
public:
    /**
     * New instance.
     */
    CFileIO();
    ~CFileIO();
    /**
     * Saves data on filesystem.
     * @param filename Name of the file.
     * @param data Data to be saved line by line.
     * @return True if successful, false otherwise.
     */
    bool Save(std::string filename, std::vector<std::string> data) const;
    /**
     * Loads a file from a given filename.
     * @param filename Name of the file.
     * @return True if successful, false otherwise.
     */
    std::vector<std::string> Load(std::string filename) const;
    /**
     * Gets a list of all filenames from Saved Games folder.
     * @return Vector of filenames.
     */
    std::vector<std::string> GetSavedGamesFilenames() const;
    /**
     * Deletes a file.
     * @param filename Name of the file.
     * @return True on success, false otherwise.
     */
    bool Delete(std::string filename) const;
    
    /*! Path to folder where savegames are stored. */
    static std::string saveFolder;
    /*! Path to folder where data files are stored. */
    static std::string dataFolder;
    
};

#endif /* FILEIO_HPP */

