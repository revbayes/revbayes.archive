#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#include <string>
#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <fstream>
#include <stdio.h>

using namespace boost;
namespace fs = boost::filesystem;

class Filesystem {
    typedef std::vector<std::string> StringVector;

public:

    static bool saveToFile(std::string filename, std::string path, std::string content) {

        // check that directory exist
        fs::path _path(path);
        if (!fs::exists(_path)) {
            if (!boost::filesystem::create_directory(_path)) {
                return false;
            }
        }

        // make a valid file path
        fs::path _file(_path / filename);

        // write out new file content
        std::fstream fs;
        fs.open(_file.string().c_str(), std::fstream::out | std::fstream::trunc);
        fs << content;
        fs.close();

        return true;
    }

    /**
     * Portable code to get system directory separator
     * 
     * @return directory separator
     */
    static std::string directorySeparator() {
        fs::path slash("/");
        return slash.make_preferred().native(); // convert to current os directory separator    
    }

    /**
     * Portable code to get full path to user home directory.
     * 
     * @param path
     * @return full path to user home directory
     */
    static std::string expandUserDir(std::string path) {
        if (not path.empty() and path[0] == '~') {
            char const* home = getenv("HOME");

            if (home or ((home = getenv("USERPROFILE")))) {
                path.replace(0, 1, home);
            }
        } else {
            char const *hdrive = getenv("HOMEDRIVE"), *hpath = getenv("HOMEPATH");
            path.replace(0, 1, std::string(hdrive) + hpath);
        }
        return path;
    }

    /**
     * 
     * @param basePath
     * @param relativePath : search expression
     * @return 
     */
    static StringVector getFileList(std::string basePath, std::string relativePath) {
        fs::path _relativePath(relativePath);
        fs::path _basePath(basePath);
        fs::path _combined = _basePath / _relativePath;

        StringVector v;

        if (!fs::exists(_combined)) {
            _combined = _combined.parent_path();
            _relativePath = _relativePath.parent_path();
            if (!fs::exists(_combined)) {
                return v; // non existing path
            } 
        }

        if (!fs::is_directory(_combined)) {
            return v; // is an existing file
        }

        fs::directory_iterator end_itr; // Default ctor yields past-the-end
        for (fs::directory_iterator i(_combined); i != end_itr; ++i) {         
            std::string s = (_relativePath.string() / i->path().filename()).string();            
            v.push_back(s);
        }

        return v;
    }


};
#endif	/* FILESYSTEM_H */
