/* 
 * File:   filesystem.h
 * Author: johadunf
 *
 * Created on May 6, 2014, 2:17 PM
 */

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
     * List all files  in the given directory.
     * 
     * @param directory     directory in which to search     
     * @return 
     */
    static StringVector getFileList(std::string basePath, std::string relativePath) {
        fs::path p(relativePath);
        fs::path bp(basePath);
        fs::path _path;        

        StringVector v;

        // construct search path
        if (p.is_relative()) {            
            _path = bp / p;            
        } else {
            _path = p;
        }

        if (!fs::exists(_path)) {

            //std::cout << "\n\rpath " << _path.string() << " does not exist\n\r";
            //std::cout << "parent path " << _path.parent_path().string() << "\n\r";
            _path = _path.parent_path();
            
            if (!fs::exists(_path)) {
                return v;
            }
            
        }
        if (!fs::is_directory(_path)) {
            //std::cout << "\n\rpath " << _path.string() << " is not a directory\n\r";
            return v;
        }
        
        fs::directory_iterator end_itr; // Default ctor yields past-the-end
        for (fs::directory_iterator i(_path); i != end_itr; ++i) {            
            v.push_back((i->path().filename()).string());            
        }
        return v;
    }


};
#endif	/* FILESYSTEM_H */
