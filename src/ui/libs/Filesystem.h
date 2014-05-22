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

using namespace boost;
namespace fs = boost::filesystem;

class Filesystem {
    typedef std::vector<std::string> StringVector;

public:

    static bool saveToFile(std::string filename, std::string path, std::string content) {

        // check that directory exist
        fs::path _path(path);
        if (!fs::exists(_path)) {
            if(!boost::filesystem::create_directory(_path)){
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
     * List all files (optionally filtered by extension) in the given list of directories.
     * 
     * @param directories   directories in which to search
     * @param extension     full extension including the dot, eg .rev, .exe, ...
     * @return 
     */
    static StringVector getFileList(StringVector directories, std::string extension = "") {
        StringVector result;

        BOOST_FOREACH(std::string s, directories) {

            BOOST_FOREACH(std::string _s, getFileList(s, extension)) {
                result.push_back(_s);
            }
        }
        return result;
    }

    /**
     * List all files (optionally filtered by extension) in the given directory.
     * 
     * @param directory     directory in which to search
     * @param extension     full extension including the dot, eg .rev, .exe, ...
     * @return 
     */
    static StringVector getFileList(std::string directory, std::string extension = "") {
        fs::path p(directory);
        std::string ext = boost::to_lower_copy(extension);
        StringVector v;
        fs::recursive_directory_iterator dir(p), end;
        while (dir != end) {
            //            if (dir->path().filename() == exclude_this_directory) {
            //                dir.no_push(); // don't recurse into this directory.
            //            }

            if (fs::is_regular_file(dir->path())) {
                if (extension == "") {
                    v.push_back(dir->path().string());
                } else if (boost::to_lower_copy(dir->path().extension().string()) == ext) {
                    v.push_back(dir->path().string());
                }
            }
            ++dir;
        }

        return v;
    }
};
#endif	/* FILESYSTEM_H */
