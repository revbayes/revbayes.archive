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

using namespace boost;
namespace fs = boost::filesystem;

static std::string directorySeparator(){
    fs::path slash("/");    
    return slash.make_preferred().native(); // convert to current os directory separator    
}

static std::string expandUserDir(std::string path) {
    if (not path.empty() and path[0] == '~') {
        //assert(path.size() == 1 or path[1] == '/');  // or other error handling
        char const* home = getenv("HOME");

        if (home or ((home = getenv("USERPROFILE")))) {
            path.replace(0, 1, home);
        }
    } else {
        char const *hdrive = getenv("HOMEDRIVE"), *hpath = getenv("HOMEPATH");
        //assert(hdrive);  // or other error handling
        //assert(hpath);
        path.replace(0, 1, std::string(hdrive) + hpath);
    }
    return path;
}

#endif	/* FILESYSTEM_H */

