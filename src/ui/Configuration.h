/* 
 * File:   Configuration.h
 * Author: johadunf
 *
 * Created on May 6, 2014, 2:53 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include <string>
#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
using namespace boost;
namespace fs = boost::filesystem;

class Configuration {
public:
    Configuration(std::string inifile);
    void parseInifile();    
    void save();
    
    bool isHelpDirOk() const;
    bool isRevHomeOk() const;
    void setHelpDir(std::string helpDir);
    std::string getHelpDir() const;
    void setRevHome(std::string revHome);
    std::string getRevHome() const;
    void setIniFile(std::string iniFile);
    std::string getIniFile() const;
    std::string getMessage() const;
private:
    void createInifile(); 
    void validate();
    
    std::string message;
    std::string iniFile;
    std::string revHome;
    std::string helpDir;
    //std::string pluginDir;
    
    bool revHomeOk;
    bool helpDirOk;
    //bool pluginDirOk;

};

#endif	/* CONFIGURATION_H */

