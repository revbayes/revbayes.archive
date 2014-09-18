/* 
 * File:   Configuration.h
 * Author: johadunf
 *
 * Created on May 6, 2014, 2:53 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include <string>

#include <boost/foreach.hpp>

#include "RbFileManager.h"
#include "RepositoryInfo.h"

#include "pugixml.hpp"

const std::string relativeHelpDir = "../../help";
const std::string relativeDownloadDir = "../../downloads";
const std::string rbExecutable = "rb-extended";
const std::string defaultConfigFile = Filesystem::expandUserDir("~") + Filesystem::directorySeparator() + "revbayes.config.xml";
const std::string defaultVersion = "0.1";

class Configuration {
public:

    Configuration() {
        setDefaults();
    }

    Configuration(std::string configFile) {
        setDefaults();
        fs::path i(configFile);
        this->configFile = i.normalize().string();
    }

    bool parseConfiguration() {
        this->message = "";

        if (!fs::exists(this->configFile)) {
            this->message.append("Initializing a new configuration file '" + this->configFile + "'.\n");
            saveConfiguration();
            validate();
            return false;
        }

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(configFile.c_str(), pugi::parse_default);
        if (result.status != pugi::status_ok) {
            this->message.append("Error: ").append(result.description()).append("\nResorting to default values...\n");
            setDefaults();
            validate();
            return false;
        }

        pugi::xpath_node_set nodeSet, subSet;

        try {

            this->revHome = doc.select_single_node("//configuration/rev-home").node().child_value();
            this->helpDir = doc.select_single_node("//configuration/help-dir").node().child_value();
            this->downloadDir = doc.select_single_node("//configuration/download-dir").node().child_value();

            this->revRepositories.clear();
            nodeSet = doc.select_nodes("//configuration/repositories/repository");
            for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
                pugi::xpath_node node = *it;
                RepositoryInfo r;
                r.SetName(node.node().child_value("name"));
                r.SetServer(node.node().child_value("server"));
                r.SetProject(node.node().child_value("project"));
                r.SetPath(node.node().child_value("path"));
                r.SetProtocol(node.node().child_value("protocol"));
                r.SetType(node.node().child_value("type"));
                this->revRepositories.push_back(r);
            }

            validate();
            return true;
        } catch (...) {
            this->message.append("Error: ").append(result.description()).append("\nResorting to default values...\n");
            setDefaults();
            validate();
            return false;
        }

    }

    void save() {
        saveConfiguration();
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

    void setConfigFile(std::string configFile) {
        this->configFile = configFile;
    }

    std::string getConfigFile() const {
        return configFile;
    }

    std::string getMessage() const {
        return message;
    }

    std::vector<RepositoryInfo> getRepositories() {
        return revRepositories;
    }

    std::vector<RepositoryInfo> getRevRepositories() const {
        return revRepositories;
    }

    bool isDownloadDirOk() const {
        return downloadDirOk;
    }

    void setDownloadDir(std::string downloadDir) {
        this->downloadDir = downloadDir;
    }

    std::string getDownloadDir() const {
        return downloadDir;
    }

private:
    std::string version;
    std::string message;
    std::string configFile;

    std::string revHome;
    bool revHomeOk;
    std::string helpDir;
    bool helpDirOk;
    std::string downloadDir;
    bool downloadDirOk;

    std::vector<RepositoryInfo> revRepositories;

    void setDefaults() {
        this->version = defaultVersion;
        this->configFile = defaultConfigFile;

        fs::path revHome(boost::filesystem::current_path());
        this->revHome = revHome.string();

        fs::path helpDir(revHome / relativeHelpDir);
        this->helpDir = helpDir.normalize().string();

        fs::path downloadDir(revHome / relativeDownloadDir);
        this->downloadDir = downloadDir.normalize().string();

        revRepositories.clear();
        RepositoryInfo *r = new RepositoryInfo;
        r->SetName("SourceForge-RevBayes");
        r->SetType("SourceForge");
        r->SetServer("sourceforge.net");
        r->SetProject("revbayes");
        r->SetPath("/p/revbayes/code-git/ci/master/tree/examples/");
        r->SetProtocol("http");
        revRepositories.push_back(*r);

        RepositoryInfo *r1 = new RepositoryInfo;
        r1->SetName("Dummy");
        r1->SetType("Dummy");
        r1->SetServer("Dummy");
        r->SetProject("Dummy");
        r->SetPath("Dummy");
        r1->SetProtocol("https");

        revRepositories.push_back(*r1);
    }

    void saveConfiguration() {
        pugi::xml_document doc;

        // root node
        pugi::xml_node node = doc.append_child("configuration");
        node.append_attribute("version") = this->version.c_str();

        // directories        
        node.append_child("help-dir").text().set(this->helpDir.c_str());
        node.append_child("rev-home").text().set(this->revHome.c_str());
        node.append_child("download-dir").text().set(this->downloadDir.c_str());

        // repositories
        pugi::xml_node repos = node.append_child("repositories");

        BOOST_FOREACH(RepositoryInfo r, revRepositories) {
            pugi::xml_node repo = repos.append_child("repository");
            repo.append_child("name").text().set(r.GetName().c_str());
            repo.append_child("server").text().set(r.GetServer().c_str());
            repo.append_child("project").text().set(r.GetProject().c_str());
            repo.append_child("path").text().set(r.GetPath().c_str());
            repo.append_child("protocol").text().set(r.GetProtocol().c_str());
            repo.append_child("type").text().set(r.GetType().c_str());
        }

        //doc.save(std::cout);
        doc.save_file(this->configFile.c_str());
    }

    void validate() {

        // validate and give the user a hint if something is wrong
        fs::path revBin(this->revHome + Filesystem::directorySeparator() + rbExecutable);
        this->revHomeOk = fs::exists(revBin);
        this->helpDirOk = fs::exists(this->helpDir);
        this->downloadDirOk = fs::exists(this->downloadDir);
        
        if (!this->revHomeOk) {
            this->message
                    .append("Warning: the path to 'revHome' specified in '" + this->configFile + "' seems invalid.\n")
                    .append("Please edit this file so 'revHome' points to the directory where the RevBayes executable is.\n");
            return;
        }

        if (!this->helpDirOk) {
            this->message
                    .append("Warning: the path to 'helpDir' specified in '" + this->configFile + "' seems invalid.\n")
                    .append("Please edit this file so 'helpDir' points to the directory where the RevBayes help files are located.\n");
            return;
        }

        if (!this->downloadDirOk) {
            this->message
                    .append("Warning: the path to 'downloadDir' specified in '" + this->configFile + "' seems invalid.\n")
                    .append("Please edit this file so 'downloadDir' points to the directory where the files are located.\n");
            return;
        }

    }

};

#endif	/* CONFIGURATION_H */

