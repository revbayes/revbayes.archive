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
#include "Filesystem.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace boost;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

const std::string relativeHelpDir = "../../help";
const std::string rbExecutable = "rb-extended";

class Configuration {
public:
    
    Configuration() {}

    Configuration(std::string inifile) {
        fs::path i(inifile);
        this->iniFile = i.normalize().string();
    }

    void parseInifile() {
        this->message = "";

        if (!fs::exists(this->iniFile)) {
            this->message.append("Initializing a new configuration file '" + this->iniFile + "'.\n");
            createInifile();
            validate();
            return;
        }

        pt::ptree p;
        try {
            pt::ini_parser::read_ini(this->iniFile, p);
            this->revHome = p.get<std::string>("directories.revHome");
            this->helpDir = p.get<std::string>("directories.helpDir");
            this->message.append("Configuration loaded from '" + this->iniFile + "'.\n");
            validate();
        } catch (...) {
            this->message.append("An error occurred while trying to parse '" + this->iniFile + "'. No configuration is loaded.\n");
        }

    }

    void save() {
        pt::ptree p;
        p.put("directories.revHome", this->revHome);
        p.put("directories.helpDir", this->helpDir);
        pt::write_ini(this->iniFile, p);
    }

    bool isHelpDirOk() const {
        return helpDirOk;
    }

    bool isRevHomeOk() const {
        return revHomeOk;
    }

    void setHelpDir(std::string helpDir) {
        this->helpDir = helpDir;
        validate();
    }

    std::string getHelpDir() const {
        return helpDir;
    }

    void setRevHome(std::string revHome) {
        this->revHome = revHome;
        validate();
    }

    std::string getRevHome() const {
        return revHome;
    }

    void setIniFile(std::string iniFile) {
        this->iniFile = iniFile;
    }

    std::string getIniFile() const {
        return iniFile;
    }

    std::string getMessage() const {
        return message;
    }
private:

    void createInifile() {

        fs::path revHome(boost::filesystem::current_path());
        this->revHome = revHome.string();

        fs::path helpDir(revHome / relativeHelpDir);
        this->helpDir = helpDir.normalize().string();

        save();
    }

    void validate() {

        // validate and give the user a hint if something is wrong
        fs::path revBin(this->revHome + Filesystem::directorySeparator() + rbExecutable);
        this->revHomeOk = fs::exists(revBin);
        this->helpDirOk = fs::exists(this->helpDir);

        if (!this->revHomeOk) {
            this->message
                    .append("Warning: the path to 'revHome' specified in '" + this->iniFile + "' seems invalid.\n")
                    .append("Please edit this file so 'revHome' points to the directory where the RevBayes executable is.\n");
            return;
        }

        if (!this->helpDirOk) {
            this->message
                    .append("Warning: the path to 'helpDir' specified in '" + this->iniFile + "' seems invalid.\n")
                    .append("Please edit this file so 'helpDir' points to the directory where the RevBayes help files are located.\n");
            return;
        }

    }

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

