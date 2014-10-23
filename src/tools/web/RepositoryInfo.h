/* 
 * File:   RevRepository.h
 * Author: johadunf
 *
 * Created on May 20, 2014, 2:08 PM
 */

#ifndef REPOSITORYINFO_H
#define	REPOSITORYINFO_H

#include <string>
#include <vector>

class RepositoryInfo {
public:

    RepositoryInfo(){
        types.push_back("SourceForge");
        types.push_back("Dummy"); 
        //...
    }
    
    void SetOfficial(std::string official) {
        this->official = official;
    }

    std::string GetOfficial() const {
        return official;
    }

    void SetProtocol(std::string protocol) {
        this->protocol = protocol;
    }

    std::string GetProtocol() const {
        return protocol;
    }

    void SetServer(std::string server) {
        this->server = server;
    }

    std::string GetServer() const {
        return server;
    }

    void SetName(std::string name) {
        this->name = name;
    }

    std::string GetName() const {
        return name;
    }

    std::string GetType() const {
        return type;
    }
    
    void SetType(std::string type) {
        this->type = type;
    }

    void SetPath(std::string path) {
        this->path = path;
    }

    std::string GetPath() const {
        return path;
    }

    void SetProject(std::string project) {
        this->project = project;
    }

    std::string GetProject() const {
        return project;
    }

private:
    std::string name;
    std::string server;
    std::string project;
    std::string path;
    std::string protocol;
    std::string official;
    std::string type;
    std::vector<std::string> types;
};

#endif	/* REPOSITORYINFO_H */

